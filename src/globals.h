#ifndef GLOBALS__H
#define GLOBALS__H

#include <Arduino.h>

// Credentials for Captive Portal
#define AP_SSID "UNINORTE_EXPOFISICA"
#define AP_PASSWORD "admin"

// General PATH
#define CONFIGURATION_PATH "/config.def"
#define MAX_CHARACTERS_CONF_LINE 40

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Uninorte expofisica 20201</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <h3>Uninorte exposifica 2021</h3>
  <br><br>
  <form action="/index.html">
    <input type="submit" value="Join network">
  </form>
</body></html>)rawliteral";

const char configuration_page_html[] PROGMEM = R"rawliteral(
<!doctype html><html lang="en"><head><meta charset="utf-8"/><link rel="icon" href="/favicon.ico"/><meta name="viewport" content="width=device-width,initial-scale=1"/><meta name="theme-color" content="#000000"/><meta name="description" content="Web site created using create-react-app"/><link rel="apple-touch-icon" href="/logo192.png"/><title>Uninorte - Expofisica</title></head><body><noscript>You need to enable JavaScript to run this app.</noscript><div id="root"></div><script>!function(e){function t(t){for(var n,i,l=t[0],a=t[1],p=t[2],c=0,s=[];c<l.length;c++)i=l[c],Object.prototype.hasOwnProperty.call(o,i)&&o[i]&&s.push(o[i][0]),o[i]=0;for(n in a)Object.prototype.hasOwnProperty.call(a,n)&&(e[n]=a[n]);for(f&&f(t);s.length;)s.shift()();return u.push.apply(u,p||[]),r()}function r(){for(var e,t=0;t<u.length;t++){for(var r=u[t],n=!0,l=1;l<r.length;l++){var a=r[l];0!==o[a]&&(n=!1)}n&&(u.splice(t--,1),e=i(i.s=r[0]))}return e}var n={},o={1:0},u=[];function i(t){if(n[t])return n[t].exports;var r=n[t]={i:t,l:!1,exports:{}};return e[t].call(r.exports,r,r.exports,i),r.l=!0,r.exports}i.m=e,i.c=n,i.d=function(e,t,r){i.o(e,t)||Object.defineProperty(e,t,{enumerable:!0,get:r})},i.r=function(e){"undefined"!=typeof Symbol&&Symbol.toStringTag&&Object.defineProperty(e,Symbol.toStringTag,{value:"Module"}),Object.defineProperty(e,"__esModule",{value:!0})},i.t=function(e,t){if(1&t&&(e=i(e)),8&t)return e;if(4&t&&"object"==typeof e&&e&&e.__esModule)return e;var r=Object.create(null);if(i.r(r),Object.defineProperty(r,"default",{enumerable:!0,value:e}),2&t&&"string"!=typeof e)for(var n in e)i.d(r,n,function(t){return e[t]}.bind(null,n));return r},i.n=function(e){var t=e&&e.__esModule?function(){return e.default}:function(){return e};return i.d(t,"a",t),t},i.o=function(e,t){return Object.prototype.hasOwnProperty.call(e,t)},i.p="/";var l=this.webpackJsonpinitial_page=this.webpackJsonpinitial_page||[],a=l.push.bind(l);l.push=t,l=l.slice();for(var p=0;p<l.length;p++)t(l[p]);var f=a;r()}([])</script><script src="/static/js/2.e3bcf270.chunk.js"></script><script src="/static/js/main.adec6976.chunk.js"></script></body></html>
)rawliteral";


#endif // GLOBALS__H
