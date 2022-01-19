const addon = require('./build/Release/testaddon.node');
const electron = require('electron')
console.log(addon.hello(46, 40)); // 'world'

function toGrayScale(){
    addon.toGray();
}