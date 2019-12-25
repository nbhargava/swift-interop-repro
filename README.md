To run the working case, open in the XCode project in `WorkingInterop` and hit run. You should see `Name: arg` show up in the log, indicating that the callback to the C-style function succeeded.

To run the non-working case from `non-working-interop/`, you need to first build the swift package:

```
swift build --package-path ./src/CallbackExample
```

To build the interop code, run:

```
yarn install
```

Finally to run the Electron app, run:

```
yarn start
```

The logs indicating that the call to fibonacci work and that the callback seems to fail will show up in the Terminal rather than in the Electron JS console.
