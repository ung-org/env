/*
 * UNG's Not GNU
 *
 * Copyright (c) 2011-2017, Jakob Kaivo <jkk@ung.org>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#define _XOPEN_SOURCE 700
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define UTILITY_NOT_INVOKED (126)
#define UTILITY_NOT_FOUND (127)

extern char **environ;

int main(int argc, char *argv[])
{
	int c;
	while ((c = getopt(argc, argv, "i")) != -1) {
		switch (c) {
		case 'i':	/** ignore inherited environment **/
			environ = NULL;
			break;

		default:
			return 1;
		}
	}

	while (argv[optind] && strchr(argv[optind], '=')) {
		putenv(argv[optind++]);
	}

	if (optind >= argc) {
		if (environ) {
			for (int i = 0; environ[i]; i++) {
				puts(environ[i]);
			}
		}
		return 0;
	}
	
	execvp(argv[optind], argv + optind);

	fprintf(stderr, "env: Couldn't run '%s': %s\n", argv[optind], strerror(errno));
	return errno == ENOENT ? UTILITY_NOT_FOUND : UTILITY_NOT_INVOKED;
}
