

const { app, BrowserWindow } = require("electron");
function createWindow() {
    // Create the browser window.
    let win = new BrowserWindow({
        width: 800,
        height: 600,
        webPreferences: {
            nodeIntegration: true,
            contextIsolation: false
        }
    });
    // and load the index.html of the app.
    win.loadFile("index.html");
}
app.on("ready", createWindow);

function toGrayScale() {
    addon = require('./build/Release/testaddon.node');
    addon.toGray();
    location.reload();
}

function turnLeft() {
    addon = require('./build/Release/testaddon.node');
    addon.turnLeft();
    location.reload();
}

function turnRight() {
    addon = require('./build/Release/testaddon.node');
    addon.turnRight();
    location.reload();
}

function flipVertical() {
    addon = require('./build/Release/testaddon.node');
    addon.flipVertical();
    location.reload();
}

function flipHorizontal() {
    addon = require('./build/Release/testaddon.node');
    addon.flipHorizontal();
    location.reload();
}

