// Generated by BUCKLESCRIPT VERSION 5.0.4, PLEASE EDIT WITH CARE
'use strict';

var Curry = require("bs-platform/lib/js/curry.js");
var React = require("react");
var ReactDom = require("react-dom");
var Belt_Array = require("bs-platform/lib/js/belt_Array.js");
var Belt_Option = require("bs-platform/lib/js/belt_Option.js");

function renderToBody(reactElement) {
  var $$document$1 = document;
  return Belt_Option.mapWithDefault(Belt_Array.get(Array.prototype.slice.call($$document$1.getElementsByTagName("body")), 0), null, (function (param) {
                return ReactDom.createPortal(reactElement, param);
              }));
}

function App$Modal(Props) {
  var isOpen = Props.isOpen;
  if (isOpen) {
    return renderToBody(React.createElement("div", undefined, "this is the modal"));
  } else {
    return null;
  }
}

var Modal = /* module */[
  /* renderToBody */renderToBody,
  /* make */App$Modal
];

function App(Props) {
  var match = React.useState((function () {
          return false;
        }));
  var setOpen = match[1];
  return React.createElement("div", undefined, React.createElement("button", {
                  onClick: (function (_e) {
                      return Curry._1(setOpen, (function (prevState) {
                                    return !prevState;
                                  }));
                    })
                }, "open the modal"), React.createElement(App$Modal, {
                  isOpen: match[0]
                }));
}

var make = App;

exports.Modal = Modal;
exports.make = make;
/* react Not a pure module */