// Generated by BUCKLESCRIPT VERSION 5.0.4, PLEASE EDIT WITH CARE
'use strict';

var Css = require("bs-css/src/Css.js");
var Curry = require("bs-platform/lib/js/curry.js");
var React = require("react");
var ReactDom = require("react-dom");
var Belt_Array = require("bs-platform/lib/js/belt_Array.js");
var Belt_Option = require("bs-platform/lib/js/belt_Option.js");
var Caml_option = require("bs-platform/lib/js/caml_option.js");
var FocusContainer$ReactHooksTemplate = require("../FocusContainer.bs.js");

Css.$$global("body", /* :: */[
      Css.margin(Css.px(0)),
      /* :: */[
        Css.position(/* relative */903134412),
        /* :: */[
          Css.minHeight(Css.vh(100)),
          /* [] */0
        ]
      ]
    ]);

function Modal$Cover(Props) {
  return React.createElement("div", {
              className: Css.style(/* :: */[
                    Css.position(/* fixed */10615156),
                    /* :: */[
                      Css.top(Css.px(0)),
                      /* :: */[
                        Css.left(Css.px(0)),
                        /* :: */[
                          Css.right(Css.px(0)),
                          /* :: */[
                            Css.bottom(Css.px(0)),
                            /* :: */[
                              Css.zIndex(500),
                              /* :: */[
                                Css.height(Css.vh(100)),
                                /* :: */[
                                  Css.width(Css.vw(100)),
                                  /* :: */[
                                    Css.background(/* `hex */[
                                          5194459,
                                          "000"
                                        ]),
                                    /* :: */[
                                      Css.opacity(0.5),
                                      /* [] */0
                                    ]
                                  ]
                                ]
                              ]
                            ]
                          ]
                        ]
                      ]
                    ]
                  ])
            });
}

var Cover = /* module */[/* make */Modal$Cover];

var make = React.forwardRef((function (Props, ref_) {
        var children = Props.children;
        var onOverlayClick = Props.onOverlayClick;
        var onEsc = Props.onEsc;
        var escapeHandler = function ($$event) {
          var match = $$event.key;
          if (match === "Escape") {
            return Curry._1(onEsc, /* () */0);
          } else {
            return /* () */0;
          }
        };
        React.useEffect((function () {
                window.addEventListener("keydown", escapeHandler);
                return (function (param) {
                          window.removeEventListener("keydown", escapeHandler);
                          return /* () */0;
                        });
              }), /* array */[]);
        var tmp = {
          className: Css.style(/* :: */[
                Css.position(/* fixed */10615156),
                /* :: */[
                  Css.top(Css.px(0)),
                  /* :: */[
                    Css.left(Css.px(0)),
                    /* :: */[
                      Css.right(Css.px(0)),
                      /* :: */[
                        Css.bottom(Css.px(0)),
                        /* :: */[
                          Css.zIndex(999),
                          /* :: */[
                            Css.height(Css.vh(100)),
                            /* :: */[
                              Css.width(Css.vw(100)),
                              /* :: */[
                                Css.display(/* flex */-1010954439),
                                /* :: */[
                                  Css.flexDirection(/* column */-963948842),
                                  /* :: */[
                                    Css.justifyContent(/* center */98248149),
                                    /* [] */0
                                  ]
                                ]
                              ]
                            ]
                          ]
                        ]
                      ]
                    ]
                  ]
                ]
              ]),
          onClick: onOverlayClick
        };
        var tmp$1 = Belt_Option.map((ref_ == null) ? undefined : Caml_option.some(ref_), (function (prim) {
                return prim;
              }));
        if (tmp$1 !== undefined) {
          tmp.ref = Caml_option.valFromOption(tmp$1);
        }
        return React.createElement("div", tmp, children);
      }));

var Overlay = /* module */[/* make */make];

function Modal$ModalContentWrapper(Props) {
  var children = Props.children;
  return React.createElement("div", {
              className: Css.style(/* :: */[
                    Css.zIndex(999),
                    /* :: */[
                      Css.background(/* `hex */[
                            5194459,
                            "fff"
                          ]),
                      /* :: */[
                        Css.maxWidth(/* `percent */[
                              -119887163,
                              50
                            ]),
                        /* :: */[
                          Css.marginLeft(/* auto */-1065951377),
                          /* :: */[
                            Css.marginRight(/* auto */-1065951377),
                            /* [] */0
                          ]
                        ]
                      ]
                    ]
                  ]),
              onClick: (function (prim) {
                  prim.stopPropagation();
                  return /* () */0;
                })
            }, children);
}

var ModalContentWrapper = /* module */[/* make */Modal$ModalContentWrapper];

function renderToBody(element, onOverlayClick, onEsc, overlayRef) {
  var $$document$1 = document;
  var partial_arg = React.createElement(FocusContainer$ReactHooksTemplate.make, {
        children: React.createElement(make, {
              children: null,
              onOverlayClick: onOverlayClick,
              onEsc: onEsc,
              ref: overlayRef
            }, React.createElement(Modal$Cover, { }), React.createElement(Modal$ModalContentWrapper, {
                  children: element
                })),
        value: /* record */[
          /* lockScroll */true,
          /* lockFocus */true,
          /* target */overlayRef
        ]
      });
  return Belt_Option.mapWithDefault(Belt_Array.get(Array.prototype.slice.call($$document$1.getElementsByTagName("body")), 0), null, (function (param) {
                return ReactDom.createPortal(partial_arg, param);
              }));
}

function Modal(Props) {
  var isOpen = Props.isOpen;
  var onOverlayClick = Props.onOverlayClick;
  var onEsc = Props.onEsc;
  var children = Props.children;
  var overlayRef = React.useRef(null);
  if (isOpen) {
    return renderToBody(children, onOverlayClick, onEsc, overlayRef);
  } else {
    return null;
  }
}

var make$1 = Modal;

exports.Cover = Cover;
exports.Overlay = Overlay;
exports.ModalContentWrapper = ModalContentWrapper;
exports.renderToBody = renderToBody;
exports.make = make$1;
/*  Not a pure module */
