

const { app, BrowserWindow } = require("electron");
function createWindow() {
    // Create the browser window.
    let win = new BrowserWindow({
        width: 800,
        height: 600,
        webPreferences: {
            nodeIntegration: true,
            contextIsolation: false,
            enableRemoteModule: true
        }
    });
    // and load the index.html of the app.
    win.loadFile("index.html");
}
app.on("ready", createWindow);

function toGrayScale() {
    addon = require('./build/Release/testaddon.node');
    addon.toGray();
}

function turnLeft() {
    addon = require('./build/Release/testaddon.node');
    addon.turnLeft();
}

function turnRight() {
    addon = require('./build/Release/testaddon.node');
    addon.turnRight();
}

function flipVertical() {
    addon = require('./build/Release/testaddon.node');
    addon.flipVertical();
}

function flipHorizontal() {
    addon = require('./build/Release/testaddon.node');
    addon.flipHorizontal();
}

