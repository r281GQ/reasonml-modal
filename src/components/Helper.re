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