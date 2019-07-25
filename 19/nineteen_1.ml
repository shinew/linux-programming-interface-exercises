open Async

let main () =
  let%bind t, _ = Async_inotify.create "." in
  let updates = Async_inotify.pipe t in
  Pipe.iter_without_pushback updates ~f:(fun x ->
      print_s [%sexp (x : Async_inotify.Event.t)])

let command = Command.async (Command.Param.return main) ~summary:""

let () = Command.run command
