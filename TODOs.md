Notable TODOs, in no particular order:

  * When loading image files, it uses the file size to determine if it's a board or a piece. This works fine for 90% of cases, but how this is determined needs to be configurable for those other 10%.
  * Figure out how we're gonna do plugins, on-board Widgets, and such. Unfortunately, s11n's plugin layer won't work on Windows (don't ask), so that one's not too useful for QBoard (but we may use it as an interim solution, since it will work (should) on Windows for plugins statically linked to the app). My initial attempts with the Qt plugins layer haven't proven fruitful (e.g. plugins not being able to resolve API symbols from the main app).
  * Add per-piece notes and an appropriate editor widget.
  * Contemplate the "droplets" concept for configuring pieces - little drag/droppable widgets which, when dropped on a piece, change some property. (The DragDropRobot example from the Qt sources shows how to do this.)
  * Since pieces are controlled almost exclusively through setting various properties, it might (theoretically) be pretty simple to add built-in Undo/Redo support to them, by simply recording the older values of any properties. This would be much simpler to do if QBoard::setProperty() were virtual (as it damned well should be). An alternative would be to save an s11n'd copy on each change, and use that for undo (redo would lose its purpose here, as a redo would restore the state from a copy of itself). It'd be more accurate (it'd capture some other side-effects caused by property changes), but it'd be relatively slow and memory-hungry.