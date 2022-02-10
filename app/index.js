

const { app, BrowserWindow } = require("electron");
function createWindow() {
    // Create the browser window.
    let win = new BrowserWindow({
        width: 800,
        height: 600,
        webPreferences: {
            nodeIntegration: true,
            enableRemoteModule: true,
            contextIsolation: false
        }
    });
    // and load the index.html of the app.
    win.loadFile("index.html");
}

app.whenReady().then(() => {
    createWindow();
})

app.on('window-all-closed', () => {
    if (process.platform != 'darwin'){
        app.quit();
    }
})
const addon = require('./build/Release/testaddon.node')
const { ipcMain } = require("electron")
ipcMain.on('toGray', (event) => {
    addon.toGray();
    event.returnValue = null;
})

ipcMain.on('turnLeft', (event) => {
    addon.turnLeft();
    event.returnValue = null;
})

ipcMain.on('turnRight', (event) => {
    addon.turnRight();
    event.returnValue = null;
})

ipcMain.on('flipVertical', (event) => {
    addon.flipVertical();
    event.returnValue = null;
})

ipcMain.on('flipHorizontal', (event) => {
    addon.flipHorizontal();
    event.returnValue = null;
})

