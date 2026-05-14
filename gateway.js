const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');

const io = require('socket.io-client');

const socket = io('https://YOUR-RENDER-URL.onrender.com');

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

  const fields = line.split(',');

  let sensorData = {};

  fields.forEach(field => {

    const parts = field.split(':');

    if (parts.length === 2) {

      sensorData[parts[0]] = parseFloat(parts[1]);
    }
  });

  console.log(sensorData);

  socket.emit('sensorData', sensorData);
});