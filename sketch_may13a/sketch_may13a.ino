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

    Serial.print("AX:");
    Serial.println(fax);

    Serial.print("AY:");
    Serial.println(fay);

    Serial.print("AZ:");
    Serial.println(faz);
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

    Serial.print("GX:");
    Serial.println(fgx);

    Serial.print("GY:");
    Serial.println(fgy);

    Serial.print("GZ:");
    Serial.println(fgz);
  }

  // ================= Magnetometer =================
  if (IMU.magneticFieldAvailable()) {
    IMU.readMagneticField(mx, my, mz);

    Serial.print("MX:"); Serial.println(mx);
    Serial.print("MY:"); Serial.println(my);
    Serial.print("MZ:"); Serial.println(mz);
  }

  // ================= Temperature =================
  float temperature = HS300x.readTemperature();
  Serial.print("TEMP:");
  Serial.println(temperature);

  // ================= Humidity =================
  float humidity = HS300x.readHumidity();
  Serial.print("HUM:");
  Serial.println(humidity);

  // ================= Pressure =================
  float pressure = BARO.readPressure();
  Serial.print("PRESS:");
  Serial.println(pressure);

  // ================= Light =================
  if (APDS.colorAvailable()) {

    int r, g, b, ambient;
    APDS.readColor(r, g, b, ambient);

    Serial.print("LIGHT:");
    Serial.println(ambient);

    Serial.print("R:");
    Serial.println(r);

    Serial.print("G:");
    Serial.println(g);

    Serial.print("B:");
    Serial.println(b);
  }

  // ================= Proximity =================
  if (APDS.proximityAvailable()) {

    int proximity = APDS.readProximity();

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
    float dB = 20.0 * log10(rms) - 25;

    Serial.print("SOUND:");
    Serial.println(dB);

    samplesRead = 0;
  }

  Serial.println("END");

  delay(500);
}
