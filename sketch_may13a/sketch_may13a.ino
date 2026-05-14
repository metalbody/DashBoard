#include <Arduino_BMI270_BMM150.h>
#include <Arduino_HS300x.h>
#include <Arduino_LPS22HB.h>
#include <Arduino_APDS9960.h>
#include <PDM.h>

short sampleBuffer[256];
volatile int samplesRead = 0;

void onPDMdata() {
  int bytesAvailable = PDM.available();
  PDM.read(sampleBuffer, bytesAvailable);
  samplesRead = bytesAvailable / 2;
}
float dB=0.0;
int ambient,r,g,b,proximity=0;
// ================= FILTER VARIABLES =================

// Low-pass filter coefficient
const float alpha = 0.2;

// Filtered accelerometer
float fax = 0, fay = 0, faz = 0;

// Filtered gyroscope
float fgx = 0, fgy = 0, fgz = 0;

// Gyro deadband threshold
const float gyroThreshold = 0.2;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("IMU_FAIL");
    while (1);
  }

  if (!HS300x.begin()) {
    Serial.println("HS300x_FAIL");
    while (1);
  }

  if (!BARO.begin()) {
    Serial.println("BARO_FAIL");
    while (1);
  }

  if (!APDS.begin()) {
    Serial.println("APDS_FAIL");
    while (1);
  }

  PDM.onReceive(onPDMdata);

  if (!PDM.begin(1, 16000)) {
    Serial.println("MIC_FAIL");
    while (1);
  }

  Serial.println("SYSTEM_READY");
}

void loop() {

  float ax, ay, az;
  float gx, gy, gz;
  float mx, my, mz;

  // ================= Accelerometer =================
  if (IMU.accelerationAvailable()) {

    IMU.readAcceleration(ax, ay, az);

    // Low-pass filter
    fax = alpha * ax + (1 - alpha) * fax;
    fay = alpha * ay + (1 - alpha) * fay;
    faz = alpha * az + (1 - alpha) * faz;

    // Noise suppression
    if (abs(fax) < 0.02) fax = 0;
    if (abs(fay) < 0.02) fay = 0;
    if (abs(faz) < 0.02) faz = 0;
  }

  // ================= Gyroscope =================
  if (IMU.gyroscopeAvailable()) {

    IMU.readGyroscope(gx, gy, gz);

    // Low-pass filter
    fgx = alpha * gx + (1 - alpha) * fgx;
    fgy = alpha * gy + (1 - alpha) * fgy;
    fgz = alpha * gz + (1 - alpha) * fgz;

    // Deadband
    if (abs(fgx) < gyroThreshold) fgx = 0;
    if (abs(fgy) < gyroThreshold) fgy = 0;
    if (abs(fgz) < gyroThreshold) fgz = 0;
  }

  // ================= Magnetometer =================
  if (IMU.magneticFieldAvailable()) {
    IMU.readMagneticField(mx, my, mz);
  }

  // ================= Temperature =================
  float temperature = HS300x.readTemperature();

  // ================= Humidity =================
  float humidity = HS300x.readHumidity();
  // ================= Pressure =================
  float pressure = BARO.readPressure();

  // ================= Light =================
  if (APDS.colorAvailable()) {

    APDS.readColor(r, g, b, ambient);
  }

  // ================= Proximity =================
  if (APDS.proximityAvailable()) {

    proximity = APDS.readProximity();

    Serial.print("PROX:");
    Serial.println(proximity);
  }

  // ================= Sound =================
  if (samplesRead) {

    float sumSquares = 0;

    for (int i = 0; i < samplesRead; i++) {

      float sample = sampleBuffer[i];

      sumSquares += sample * sample;
    }

    // RMS amplitude
    float rms = sqrt(sumSquares / samplesRead);

    // Prevent log(0)
    if (rms < 1) rms = 1;

    // Approximate dB conversion
    // Tuned so quiet room ≈ 48 dB
    dB = 20.0 * log10(rms) - 25;

    samplesRead = 0;
  }
  Serial.print("GX:");
  Serial.print(fgx);

  Serial.print(",GY:");
  Serial.print(fgy);

  Serial.print(",GZ:");
  Serial.print(fgy);

  Serial.print(",AX:");
  Serial.print(fax);

  Serial.print(",AY:");
  Serial.print(fay);

  Serial.print(",AZ:");
  Serial.print(faz);

  Serial.print(",MX:");
  Serial.print(mx);

  Serial.print(",MY:");
  Serial.print(my);

  Serial.print(",MZ:");
  Serial.print(mz);

  Serial.print(",TEMP:");
  Serial.print(temperature);

  Serial.print(",HUM:");
  Serial.print(humidity);

  Serial.print(",PRESS:");
  Serial.print(pressure);

  Serial.print(",SOUND:");
  Serial.println(dB);
  /*
  Serial.print("LIGHT:");
  Serial.println(ambient);

  Serial.print("R:");
  Serial.println(r);

  Serial.print("G:");
  Serial.println(g);

  Serial.print("B:");
  Serial.println(b);

  Serial.print("PROX:");
  Serial.println(proximity);
  */

  Serial.println("END");

  delay(500);
}
