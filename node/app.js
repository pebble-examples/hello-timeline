'use strict';

var express = require('express');
var randomstring = require('randomstring');
var Timeline = require('pebble-api');
var util = require('util');

var app = express();
app.set('port', (process.env.PORT || 5000));

// handler for GET /
app.get('/', function (req, res) {
  res.send('Hello, World!');
});

var timeline = new Timeline();

// handler for GET /senduserpin/:userToken/:minutesToAdd?
app.get('/senduserpin/:userToken/:minutesToAdd?', function (req, res) {

  var userToken = req.params.userToken;
  var minutesToAdd = req.params.minutesToAdd || 0;

  console.log(util.format('Got a request from %s to send a pin in %d min',
    userToken, minutesToAdd));

  // generate a random id
  var id = randomstring.generate(8);

  var now = new Date();

  // create the pin object
  var pin = new Timeline.Pin({
    id: id,
    time: new Date(now.getTime() + (minutesToAdd * 60 * 1000)),
    layout: {
      type: Timeline.Pin.LayoutType.GENERIC_PIN,
      tinyIcon: Timeline.Pin.Icon.PIN,
      title: 'This is a generic pin!',
      body: 'Pin ID: ' + id
    }
  });

  // add actions to the pin
  [10, 20, 30].forEach(function (min) {
    pin.addAction(new Timeline.Pin.Action({
      type: Timeline.Pin.ActionType.OPEN_WATCH_APP,
      title: min + ' min',
      launchCode: min
    }));
  });

  // send the pin
  timeline.sendUserPin(userToken, pin, function (err, body, resp) {
    if (err) {
      return console.error(err);
    }

    res.send('Status code: ' + resp.statusCode);
  });
});

// start the webserver
app.listen(app.get('port'), function () {
  console.log('timeline-user example app listening on port %s', app.get('port'));
});
