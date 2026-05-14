const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');

const io = require('socket.io-client');

const socket = io('https://dashboard-33jw.onrender.com');

const port = new SerialPort({
  path: 'COM14',
  baudRate: 115200
});

const parser = port.pipe(
  new ReadlineParser({ delimiter: '\n' })
);

let sensorData = {};

parser.on('data', (line) => {

  line = line.trim();

  // Ignore empty lines
  if (!line) return;

  // Ignore END marker
  if (line === 'END') return;

  const fields = line.split(',');

  let sensorData = {};

  fields.forEach(field => {

    const parts = field.split(':');

    if (parts.length === 2) {

      const key = parts[0];

      const value = parseFloat(parts[1]);

      if (!isNaN(value)) {
        sensorData[key] = value;
      }
    }
  });

  // Ignore invalid packets
  if (Object.keys(sensorData).length === 0) return;

  console.log(sensorData);

  socket.emit('sensorData', sensorData);
});