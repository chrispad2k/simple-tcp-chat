# Simple TCP Chat

## Building

The build process of this project includes CMake and Ninja. You can either use VS Code Tasks or build manually.

### Building using VS Code Tasks

There are two tasks configured. The first task is called ```cmake```. This task creates and configures the ```./build``` directory, which is required for Ninja to compile. This task must be executed before the actual compilation can be started.

After that, you can compile using the task ```ninja```, which also is the default build task, btw.

### Building Manually

First you have to create the ```./build``` directory for the ninja building process:

```
cmake -G Ninja -B build .
```

After that you can compile the project:

```
ninja -C build
```

## Debugging

You can debug the app using the VS Code Debugger. There are two launch configurations you can use. The ```server``` configuration starts a new instance of the server and the ```client``` configuration starts a new instance of the client.
