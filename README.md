# Hello Timeline Example

This Hello Timeline example app shows how to use the 
[Pebble timeline API][timeline-guides] with user tokens.

![](screenshots/screenshot.png)


## Getting Started

Start by cloning this repository to your local machine.

```
git clone https://github.com/pebble-examples/hello-timeline.git
```

Generate a new UUID for your copy of this project, and copy it into
`appinfo.json`.

```
uuidgen
```

Enable the timeline for the watchapp by uploading it to the 
[dev-portal](https://dev-portal.getpebble.com/) and clicking 'Enable timeline'. 
This enables the timeline API to identify the app.

Build the Pebble app and run it on the Basalt emulator.

```
cd hello-timeline/pebble/
pebble build
pebble install --emulator basalt
```

Launch the Node.js backend application.

```
cd hello-timeline/node/ 
npm install
node app.js
```

Launch the app on the emulator. You should see a message telling you that a Pin
has been sent to your timeline. Exit the app and press down to view the
timeline. You should see your pin! (You may have to wait a minute or two and
then exit and reload the timeline to see the pin.)

## Learning More

If you want to learn more about the new Pebble timeline API, check out our
second example demonstrating the usage of the shared pins API. You should also
take a look at the 
[timeline guides on the Pebble developer website][timeline-guides].

## License

[MIT](./LICENSE)

[timeline-guides]: https://developer.getpebble.com/guides/timeline/
