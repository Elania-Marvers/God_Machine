#include "lib.h"

t_bunny_response event_handler(t_bunny_event_state state, t_bunny_keysym key, __attribute__((unused))void *data)
{   
  return state == GO_DOWN && key == BKS_ESCAPE ? EXIT_ON_SUCCESS : GO_ON;
}

t_bunny_response __loop (void *data)
{
  (void) data;
  return GO_ON;
}

t_bunny_response __display(void *data)
{
  Panda* d = (Panda *)data;
  d->clear(COLOR(40, 0, 0, 0));

  d->god_machine(COLOR(255, 0, 255, 0));

  bunny_blit (&d->_win->buffer, &d->_px->clipable, NULL);
  bunny_display(d->_win);
  return GO_ON;
}

