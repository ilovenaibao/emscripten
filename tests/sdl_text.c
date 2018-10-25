/*
 * Copyright 2013 The Emscripten Authors.  All rights reserved.
 * Emscripten is available under two separate licenses, the MIT license and the
 * University of Illinois/NCSA Open Source License.  Both these licenses can be
 * found in the LICENSE file.
 */

#include <SDL/SDL.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <emscripten.h>

int result = 0;

void one() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_TEXTEDITING: assert(0); break;
      case SDL_TEXTINPUT:
        printf("Received %s\n", event.text.text);
        if (!strcmp("a", event.text.text)) {
          result = 1;
        } else if (!strcmp("A", event.text.text)) {
          REPORT_RESULT(result);
          emscripten_run_script("throw 'done'");
        }
        break;
      default: /* Report an unhandled event */
        printf("I don't know what this event is!\n");
    }
  }
}

int main() {
  printf("start1\n");
  SDL_Init(SDL_INIT_VIDEO);
  printf("start2\n");
  SDL_SetVideoMode(600, 450, 32, SDL_HWSURFACE);
  printf("start3\n");
  SDL_StartTextInput();
  printf("start4\n");

  emscripten_run_script("simulateKeyEvent('a'.charCodeAt(0))"); // a
  printf("start5\n");
  emscripten_run_script("simulateKeyEvent('A'.charCodeAt(0))"); // A
  printf("start6\n");

  one();
  printf("start7\n");

  return 0;
}
