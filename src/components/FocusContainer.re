[@bs.module "uuid"] external uuid: unit => string = "v4";

type t = {
  lockScroll: bool,
  lockFocus: bool,
  target: ReactDOMRe.Ref.currentDomRef,
  preventTabEscape: bool,
};

let isFocusAble: Js.Re.t = [%bs.re
  "/(svg|a|area|input|select|textarea|button|iframe|div)$/"
];

let isFocusAble2: Js.Re.t = [%bs.re
  "/(input|select|textarea|a[href]|button|[tabindex]|audio[controls]|video[controls])$/"
];

let check = Js.Re.test_(isFocusAble);
let check2 = Js.Re.test_(isFocusAble2);

module Inner = {
  let defaultRef = React.createRef();

  let focusContext =
    React.createContext({
      lockFocus: true,
      lockScroll: true,
      target: defaultRef,
      preventTabEscape: true,
    });

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
  let (prevActiveElement, setPrevActiveElement) = React.useState(() => false);

  let focusTracker = React.useRef(Belt.HashMap.String.make(~hintSize=20));

  let firstElement = React.useRef(Js.Nullable.null);

  let lastElement = React.useRef(Js.Nullable.null);

  React.useEffect1(
    () => {
      switch (value.lockScroll) {
      | true => Helper.Lock->Helper.setScrollLock
      | _ => ()
      };

      Some(() => Helper.Unlock->Helper.setScrollLock);
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
            ->Belt.Option.map(Webapi.Dom.Element.getElementsByTagName("*"))
            ->Belt.Option.mapWithDefault(
                [||],
                Webapi.Dom.HtmlCollection.toArray,
              )
            ->Belt.Array.keep(x =>
                x->Webapi.Dom.Element.tagName->Js.String.toLowerCase->check
              )
            ->Belt.Array.keep(x =>
                Webapi.Dom.Element.getAttribute("tabindex", x)
                ->Belt.Option.mapWithDefault(true, x => x !== "-1")
              )
            ->Belt.Array.forEach(x => {
                React.Ref.current(focusTracker)
                ->Belt.HashMap.String.set(
                    uuid(),
                    (x, Webapi.Dom.Element.getAttribute("tabindex", x)),
                  );

                Webapi.Dom.Element.setAttribute("tabindex", "-1", x);
              })
          );

      Js.Global.setTimeout(() => ()->process->ignore, 10)->ignore;

      Some(
        () =>
          React.Ref.current(focusTracker)
          ->Belt.HashMap.String.forEach((_key, (element, value)) =>
              switch (value) {
              | Some(prevTabIndex) =>
                Webapi.Dom.Element.setAttribute(
                  "tabindex",
                  prevTabIndex,
                  element,
                )
              | None =>
                Webapi.Dom.Element.removeAttribute("tabindex", element)
              }
            ),
      );
    },
    [|prevActiveElement|],
  );

  React.useEffect1(
    () => {
      let focusAbleInsideElements =
        value.target
        ->React.Ref.current
        ->Js.Nullable.toOption
        ->Belt.Option.map(Webapi.Dom.Element.getElementsByTagName("*"))
        ->Belt.Option.mapWithDefault([||], Webapi.Dom.HtmlCollection.toArray)
        ->Belt.Array.keep(x =>
            x->Webapi.Dom.Element.tagName->Js.String.toLowerCase->check2
          );

      lastElement->React.Ref.setCurrent(
        focusAbleInsideElements
        ->Belt.Array.get(focusAbleInsideElements->Belt.Array.length - 1)
        ->Belt.Option.mapWithDefault(
            lastElement->React.Ref.current,
            Js.Nullable.return,
          ),
      );

      firstElement->React.Ref.setCurrent(
        focusAbleInsideElements
        ->Belt.Array.get(0)
        ->Belt.Option.mapWithDefault(
            firstElement->React.Ref.current,
            Js.Nullable.return,
          ),
      );

      firstElement
      ->React.Ref.current
      ->Js.Nullable.toOption
      ->Belt.Option.map(Webapi.Dom.Element.unsafeAsHtmlElement)
      ->Belt.Option.mapWithDefault((), Webapi.Dom.HtmlElement.focus);

      let trapTabKey = e => {
        switch (
          firstElement->React.Ref.current->Js.Nullable.toOption,
          e->Webapi.Dom.KeyboardEvent.shiftKey,
          value.preventTabEscape,
        ) {
        | (Some(t), true, true)
            when
              e
              ->Webapi.Dom.KeyboardEvent.target
              ->Webapi.Dom.EventTarget.unsafeAsElement
              === t =>
          e->Webapi.Dom.KeyboardEvent.preventDefault
        | (_, _, _) => ()
        };

        switch (
          lastElement->React.Ref.current->Js.Nullable.toOption,
          !e->Webapi.Dom.KeyboardEvent.shiftKey,
          value.preventTabEscape,
        ) {
        | (Some(t), true, true)
            when
              e
              ->Webapi.Dom.KeyboardEvent.target
              ->Webapi.Dom.EventTarget.unsafeAsElement
              === t =>
          e->Webapi.Dom.KeyboardEvent.preventDefault
        | (_, _, _) => ()
        };
      };

      Webapi.Dom.document
      ->Webapi.Dom.Document.asHtmlDocument
      ->Belt.Option.flatMap(Webapi.Dom.HtmlDocument.body)
      ->Belt.Option.flatMap(Webapi.Dom.Element.asHtmlElement)
      ->Belt.Option.mapWithDefault(
          (),
          Webapi.Dom.HtmlElement.addKeyDownEventListener(trapTabKey),
        );

      Some(
        () =>
          Webapi.Dom.document
          ->Webapi.Dom.Document.asHtmlDocument
          ->Belt.Option.flatMap(Webapi.Dom.HtmlDocument.body)
          ->Belt.Option.flatMap(Webapi.Dom.Element.asHtmlElement)
          ->Belt.Option.mapWithDefault(
              (),
              Webapi.Dom.HtmlElement.removeKeyDownEventListener(trapTabKey),
            ),
      );
    },
    [|prevActiveElement|],
  );

  React.useEffect1(
    () => {
      let prevFocusedElement =
        Webapi.Dom.document
        ->Webapi.Dom.Document.asHtmlDocument
        ->Belt.Option.flatMap(Webapi.Dom.HtmlDocument.activeElement);

      setPrevActiveElement(_ => true);

      Some(
        () =>
          switch (prevFocusedElement) {
          | Some(element) =>
            element
            ->Webapi.Dom.Element.unsafeAsHtmlElement
            ->Webapi.Dom.HtmlElement.focus
          | None => ()
          },
      );
    },
    [||],
  );

  prevActiveElement ? <Inner value> children </Inner> : React.null;
};