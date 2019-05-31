Css.(
  global("body", [0->px->margin, `relative->position, 100.->vh->minHeight])
);

[@bs.module "uuid"] external uuid: unit => string = "v4";

module Modal = {
  module Helper = {
    type overFlow =
      | Lock
      | Unlock;

    let convert =
      fun
      | Lock => "hidden"
      | Unlock => "auto";

    let setScrollLock = value =>
      Webapi.Dom.document
      ->Webapi.Dom.Document.asHtmlDocument
      ->Belt.Option.flatMap(Webapi.Dom.HtmlDocument.body)
      ->Belt.Option.flatMap(Webapi.Dom.Element.asHtmlElement)
      ->Belt.Option.mapWithDefault(
          (),
          Webapi.Dom.HtmlElement.setAttribute(
            "style",
            "overflow:" ++ value->convert,
          ),
        );
  };

  module FocusContainer = {
    type t = {
      lockScroll: bool,
      lockFocus: bool,
      target: ReactDOMRe.Ref.currentDomRef,
    };

    let f: Js.Re.t = [%bs.re
      "/(svg|a|area|input|select|textarea|button|iframe|div)$/"
    ];

    let check = Js.Re.test_(f);

    module Inner = {
      let u = React.createRef();

      let focusContext =
        React.createContext({lockFocus: true, lockScroll: true, target: u});

      let make = React.Context.provider(focusContext);
      [@bs.obj]
      external makeProps:
        (~value: t, ~children: React.element, ~key: string=?, unit) =>
        {
          .
          "value": t,
          "children": React.element,
        } =
        "";
    };

    [@react.component]
    let make = (~children, ~value: t) => {
      let focusTracker =
        React.useRef(Belt.HashMap.String.make(~hintSize=20));

      React.useEffect1(
        () => {
          open Helper;

          switch (value.lockScroll) {
          | true => Lock->setScrollLock
          | _ => ()
          };

          Some(() => Unlock->setScrollLock);
        },
        [||],
      );

      React.useEffect1(
        () => {
          let process = () =>
            Webapi.Dom.document
            ->Webapi.Dom.Document.asHtmlDocument
            ->Belt.Option.flatMap(Webapi.Dom.HtmlDocument.body)
            ->Belt.Option.flatMap(Webapi.Dom.Element.asHtmlElement)
            ->Belt.Option.map(Webapi.Dom.HtmlElement.childNodes)
            ->Belt.Option.mapWithDefault([||], Webapi.Dom.NodeList.toArray)
            ->Belt.Array.keep(node =>
                switch (value.target->React.Ref.current->Js.Nullable.toOption) {
                | Some(overlay) => !Webapi.Dom.Node.contains(overlay, node)
                | None => true
                }
              )
            ->Belt.Array.forEach(node =>
                node
                ->Webapi.Dom.Element.ofNode
                ->Belt.Option.map(
                    Webapi.Dom.Element.getElementsByTagName("*"),
                  )
                ->Belt.Option.mapWithDefault(
                    [||],
                    Webapi.Dom.HtmlCollection.toArray,
                  )
                // ->Belt.Array.map(Webapi.Dom.Element.unsafeAsHtmlElement)
                ->Belt.Array.keep(x =>
                    x->Webapi.Dom.Element.tagName->Js.String.toLowerCase->check
                  )
                // Webapi.Dom.Element.hasAttribute("tabindex"),
                ->Belt.Array.keep(x =>
                    Webapi.Dom.Element.getAttribute("tabindex", x)
                    ->Belt.Option.mapWithDefault(true, x => x !== "-1")
                  )
                ->Belt.Array.map(x => {
                    Js.log(x);
                    x;
                  })
                ->Belt.Array.forEach(x => {
                    React.Ref.current(focusTracker)
                    ->Belt.HashMap.String.set(uuid(), x);

                    Webapi.Dom.Element.setAttribute("tabindex", "-1", x);
                  })
              );

          Js.Global.setTimeout(() => ()->process->ignore, 10)->ignore;

          None;
        },
        [||],
      );

      <Inner value> children </Inner>;
    };
  };

  module Cover = {
    [@react.component]
    let make = () => {
      <div
        className=Css.(
          style([
            `fixed->position,
            0->px->top,
            0->px->left,
            0->px->right,
            0->px->bottom,
            500->zIndex,
            100.->vh->height,
            100.->vw->width,
            background(`hex("000")),
            opacity(0.5),
          ])
        )
      />;
    };
  };

  module Overlay = {
    [@react.component]
    let make =
      React.forwardRef((~children, ~onOverlayClick, ~onEsc, ref_) => {
        // let overlayRef = React.useRef(Js.Nullable.null);

        let escapeHandler = event =>
          switch (event->Webapi.Dom.KeyboardEvent.key) {
          | "Escape" => onEsc()
          | _ => ()
          };

        React.useEffect1(
          () => {
            Webapi.Dom.window
            |> Webapi.Dom.Window.addKeyDownEventListener(escapeHandler);

            Some(
              () =>
                Webapi.Dom.window
                |> Webapi.Dom.Window.removeKeyDownEventListener(escapeHandler),
            );
          },
          [||],
        );

        <div
          ref=?{
            ref_->Js.Nullable.toOption->Belt.Option.map(ReactDOMRe.Ref.domRef)
          }
          onClick=onOverlayClick
          className=Css.(
            style([
              `fixed->position,
              0->px->top,
              0->px->left,
              0->px->right,
              0->px->bottom,
              999->zIndex,
              100.->vh->height,
              100.->vw->width,
              display(`flex),
              flexDirection(`column),
              justifyContent(`center),
            ])
          )>
          children
        </div>;
      });
  };

  module ModalContentWrapper = {
    [@react.component]
    let make = (~children) => {
      <div
        onClick=ReactEvent.Mouse.stopPropagation
        className=Css.(
          style([
            999->zIndex,
            background(`hex("fff")),
            maxWidth(`percent(50.)),
            marginLeft(`auto),
            marginRight(`auto),
          ])
        )>
        children
      </div>;
    };
  };

  let renderToBody = (~element, ~onOverlayClick, ~onEsc, ~overlayRef) => {
    let document = Webapi.Dom.document;

    "body"
    ->Webapi.Dom.Document.getElementsByTagName(document)
    ->Webapi.Dom.HtmlCollection.toArray
    ->Belt.Array.get(0)
    ->Belt.Option.mapWithDefault(
        React.null,
        ReactDOMRe.createPortal(
          <FocusContainer
            value={lockFocus: true, lockScroll: true, target: overlayRef}>
            <Overlay
              onOverlayClick onEsc ref={overlayRef->ReactDOMRe.Ref.domRef}>
              <Cover />
              <ModalContentWrapper> element </ModalContentWrapper>
            </Overlay>
          </FocusContainer>,
        ),
      );
  };

  [@react.component]
  let make = (~isOpen: bool, ~onOverlayClick, ~onEsc, ~children) => {
    let overlayRef = React.useRef(Js.Nullable.null);

    isOpen
      ? renderToBody(~element=children, ~onOverlayClick, ~onEsc, ~overlayRef)
      : React.null;
  };
};

[@react.component]
let make = () => {
  let (isOpen, setOpen) = React.useState(() => false);

  <div className=Css.(style([4000->px->height]))>
    <div>
      <button onClick={_e => setOpen(_ => true)}>
        "open the modal"->React.string
      </button>
    </div>
    <div>
      <button onClick={_e => setOpen(_ => true)}>
        "open the modal"->React.string
      </button>
    </div>
    <div>
      <button onClick={_e => setOpen(_ => true)}>
        "open the modal"->React.string
      </button>
    </div>
    <div>
      <button onClick={_e => setOpen(_ => true)}>
        "open the modal"->React.string
      </button>
    </div>
    <Modal
      isOpen
      onOverlayClick={_e => setOpen(_ => false)}
      onEsc={_e => setOpen(_ => false)}>
      <div> "this is the modal content"->React.string </div>
    </Modal>
  </div>;
};