/*
 * UNG's Not GNU
 *
 * Copyright (c) 2011-2019, Jakob Kaivo <jkk@ung.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#define _XOPEN_SOURCE 500
#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define UTILITY_NOT_INVOKED (126)
#define UTILITY_NOT_FOUND (127)

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "");
	extern char **environ;

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

	fprintf(stderr, "env: %s: %s\n", argv[optind], strerror(errno));
	return errno == ENOENT ? UTILITY_NOT_FOUND : UTILITY_NOT_INVOKED;
}
