

addon = require('../middle-api/build/Release/testaddon.node');

addon.hello(90, 90);
//toGrayScale();



async function toGrayScale() {
    addon = require('../middle-api/build/Release/testaddon.node');
    addon.toGray();
}