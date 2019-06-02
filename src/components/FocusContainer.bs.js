// Generated by BUCKLESCRIPT VERSION 5.0.4, PLEASE EDIT WITH CARE
'use strict';

var Uuid = require("uuid");
var Curry = require("bs-platform/lib/js/curry.js");
var React = require("react");
var Belt_Array = require("bs-platform/lib/js/belt_Array.js");
var Belt_Option = require("bs-platform/lib/js/belt_Option.js");
var Caml_option = require("bs-platform/lib/js/caml_option.js");
var Belt_HashMapString = require("bs-platform/lib/js/belt_HashMapString.js");
var Webapi__Dom__Element = require("bs-webapi/src/Webapi/Webapi__Dom/Webapi__Dom__Element.js");
var Webapi__Dom__Document = require("bs-webapi/src/Webapi/Webapi__Dom/Webapi__Dom__Document.js");
var Helper$ReactHooksTemplate = require("./Helper.bs.js");

var isFocusAble = (/(svg|a|area|input|select|textarea|button|iframe|div)$/);

var isFocusAble2 = (/(input|select|textarea|a[href]|button|[tabindex]|audio[controls]|video[controls])$/);

function check(param) {
  return isFocusAble.test(param);
}

function check2(param) {
  return isFocusAble2.test(param);
}

var defaultRef = React.createRef();

var focusContext = React.createContext(/* record */[
      /* lockScroll */true,
      /* lockFocus */true,
      /* target */defaultRef,
      /* preventTabEscape */true
    ]);

var make = focusContext.Provider;

var Inner = /* module */[
  /* defaultRef */defaultRef,
  /* focusContext */focusContext,
  /* make */make
];

function FocusContainer(Props) {
  var children = Props.children;
  var value = Props.value;
  var match = React.useState((function () {
          return false;
        }));
  var setPrevActiveElement = match[1];
  var prevActiveElement = match[0];
  var focusTracker = React.useRef(Belt_HashMapString.make(20));
  var firstElement = React.useRef(null);
  var lastElement = React.useRef(null);
  React.useEffect((function () {
          var match = value[/* lockScroll */0];
          if (match) {
            Helper$ReactHooksTemplate.setScrollLock(/* Lock */0);
          }
          return (function (param) {
                    return Helper$ReactHooksTemplate.setScrollLock(/* Unlock */1);
                  });
        }), /* array */[]);
  React.useEffect((function () {
          var $$process = function (param) {
            return Belt_Array.forEach(Belt_Array.keep(Belt_Option.mapWithDefault(Belt_Option.map(Belt_Option.flatMap(Belt_Option.flatMap(Webapi__Dom__Document.asHtmlDocument(document), (function (prim) {
                                              return Caml_option.nullable_to_opt(prim.body);
                                            })), Webapi__Dom__Element.asHtmlElement), (function (prim) {
                                      return prim.childNodes;
                                    })), /* array */[], (function (prim) {
                                  return Array.prototype.slice.call(prim);
                                })), (function (node) {
                              var match = value[/* target */2].current;
                              if (match == null) {
                                return true;
                              } else {
                                return !node.contains(match);
                              }
                            })), (function (node) {
                          return Belt_Array.forEach(Belt_Array.keep(Belt_Array.keep(Belt_Option.mapWithDefault(Belt_Option.map(Webapi__Dom__Element.ofNode(node), (function (param) {
                                                        return param.getElementsByTagName("*");
                                                      })), /* array */[], (function (prim) {
                                                    return Array.prototype.slice.call(prim);
                                                  })), (function (x) {
                                                return check(x.tagName.toLowerCase());
                                              })), (function (x) {
                                            return Belt_Option.mapWithDefault(Caml_option.nullable_to_opt(x.getAttribute("tabindex")), true, (function (x) {
                                                          return x !== "-1";
                                                        }));
                                          })), (function (x) {
                                        Belt_HashMapString.set(focusTracker.current, Uuid.v4(), /* tuple */[
                                              x,
                                              Caml_option.nullable_to_opt(x.getAttribute("tabindex"))
                                            ]);
                                        x.setAttribute("tabindex", "-1");
                                        return /* () */0;
                                      }));
                        }));
          };
          setTimeout((function (param) {
                  $$process(/* () */0);
                  return /* () */0;
                }), 10);
          return (function (param) {
                    return Belt_HashMapString.forEach(focusTracker.current, (function (_key, param) {
                                  var value = param[1];
                                  var element = param[0];
                                  if (value !== undefined) {
                                    element.setAttribute("tabindex", value);
                                    return /* () */0;
                                  } else {
                                    element.removeAttribute("tabindex");
                                    return /* () */0;
                                  }
                                }));
                  });
        }), /* array */[prevActiveElement]);
  React.useEffect((function () {
          var focusAbleInsideElements = Belt_Array.keep(Belt_Option.mapWithDefault(Belt_Option.map(Caml_option.nullable_to_opt(value[/* target */2].current), (function (param) {
                          return param.getElementsByTagName("*");
                        })), /* array */[], (function (prim) {
                      return Array.prototype.slice.call(prim);
                    })), (function (x) {
                  return check2(x.tagName.toLowerCase());
                }));
          lastElement.current = Belt_Option.mapWithDefault(Belt_Array.get(focusAbleInsideElements, focusAbleInsideElements.length - 1 | 0), lastElement.current, (function (prim) {
                  return prim;
                }));
          firstElement.current = Belt_Option.mapWithDefault(Belt_Array.get(focusAbleInsideElements, 0), firstElement.current, (function (prim) {
                  return prim;
                }));
          Belt_Option.mapWithDefault(Belt_Option.map(Caml_option.nullable_to_opt(firstElement.current), (function (prim) {
                      return prim;
                    })), /* () */0, (function (prim) {
                  prim.focus();
                  return /* () */0;
                }));
          var trapTabKey = function (e) {
            var match = firstElement.current;
            var match$1 = e.shiftKey;
            var match$2 = value[/* preventTabEscape */3];
            if (!(match == null)) {
              if (match$1 && match$2 && e.target === match) {
                e.preventDefault();
              }
              
            }
            var match$3 = lastElement.current;
            var match$4 = !e.shiftKey;
            var match$5 = value[/* preventTabEscape */3];
            if (!(match$3 == null) && match$4 && match$5 && e.target === match$3) {
              e.preventDefault();
              return /* () */0;
            } else {
              return /* () */0;
            }
          };
          Belt_Option.mapWithDefault(Belt_Option.flatMap(Belt_Option.flatMap(Webapi__Dom__Document.asHtmlDocument(document), (function (prim) {
                          return Caml_option.nullable_to_opt(prim.body);
                        })), Webapi__Dom__Element.asHtmlElement), /* () */0, (function (param) {
                  param.addEventListener("keydown", trapTabKey);
                  return /* () */0;
                }));
          return (function (param) {
                    return Belt_Option.mapWithDefault(Belt_Option.flatMap(Belt_Option.flatMap(Webapi__Dom__Document.asHtmlDocument(document), (function (prim) {
                                          return Caml_option.nullable_to_opt(prim.body);
                                        })), Webapi__Dom__Element.asHtmlElement), /* () */0, (function (param) {
                                  param.removeEventListener("keydown", trapTabKey);
                                  return /* () */0;
                                }));
                  });
        }), /* array */[prevActiveElement]);
  React.useEffect((function () {
          var prevFocusedElement = Belt_Option.flatMap(Webapi__Dom__Document.asHtmlDocument(document), (function (prim) {
                  return Caml_option.nullable_to_opt(prim.activeElement);
                }));
          Curry._1(setPrevActiveElement, (function (param) {
                  return true;
                }));
          return (function (param) {
                    if (prevFocusedElement !== undefined) {
                      Caml_option.valFromOption(prevFocusedElement).focus();
                      return /* () */0;
                    } else {
                      return /* () */0;
                    }
                  });
        }), /* array */[]);
  if (prevActiveElement) {
    return React.createElement(make, {
                value: value,
                children: children
              });
  } else {
    return null;
  }
}

var make$1 = FocusContainer;

exports.isFocusAble = isFocusAble;
exports.isFocusAble2 = isFocusAble2;
exports.check = check;
exports.check2 = check2;
exports.Inner = Inner;
exports.make = make$1;
/* isFocusAble Not a pure module */
