#ifndef ASYNC_H
#define ASYNC_H
/*
 * Copyright (c) 2019, Sandro Magi
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the async.h library.
 *
 * Author: Sandro Magi <naasking@gmail.com>
 *
 */

/*
 * = Stackless Async Subroutines =
 *
 * Taking inspiration from protothreads and async/await as found in C#, Rust and JS,
 * this is an async/await implementation for C based on Duff's device.
 *
 * Features:
 *
 * 1. It's a bit simpler to follow than protothreads because the async state
 *    is caller-saved rather than callee-saved.
 * 2. Subroutines can have persistent state that isn't just static state, because
 *    each async subroutine accepts its own struct it uses as a parameter, and
 *    the async state is stored there.
 * 3. Because of the more flexible state, async subroutines can be nested
 *    in tree-like fashion which permits fork/join concurrency patterns.
 *
 * Caveats:
 *
 * 1. Due to compile-time build error, MSVC requires changing:
 *     Project Properties > Configuration Properties > C/C++ > General > Debug Information Format
 *    From "Program Database for Edit And Continue" to "Program Database".
 */

/*
 * The generator event status
 */
typedef enum ASYNC_EVT { ASYNC_INIT = 0, ASYNC_DONE = -1 } async;

/*
 * Declare the async state
 */
#define async_state unsigned _async_kcont

/*
 * Core async structure, optional to use.
 */
struct async { async_state; };

/*
 * Mark the start of an async subroutine
 */
#define async_begin(k) switch(k->_async_kcont) { case 0:

/*
 * Mark the end of a generator thread
 */
#define async_end default: return ASYNC_DONE; }

/*
 * Wait until the condition succeeds
 */
#define await(cond) await_while(!(cond))

/*
 * Wait while the condition succeeds (optional)
 */
//#define await_while(cond) while(cond) {async_yield;}
#define await_while(cond) case __LINE__: if (cond) return __LINE__

/*
 * Yield execution
 */
#define async_yield return __LINE__; case __LINE__:

/*
 * Exit the current async subroutine
 */
#define async_exit return ASYNC_DONE

/*
 * Initialize a new async computation
 */
#define async_init(state) (state)->_async_kcont=ASYNC_INIT

/*
 * Check if async subroutine is done
 */
#define async_done(state) (state)->_async_kcont==ASYNC_DONE

/*
 * Resume a running async computation and check for completion
 */
#define async_call(f, state) (async_done(state) || ASYNC_DONE==((state)->_async_kcont = (f)(state)))

#endif