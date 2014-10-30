#- Makefile --
#-	This is the make file for the geog libraries and application.
#-	
# Copyright (c) 2011, Gordon D. Carrie. All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 
#     * Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
# TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# Please send feedback to dev0@trekix.net
#
# $Revision: 1.11 $ $Date: 2013/01/10 21:17:25 $

# Where to install things
PREFIX=${HOME}/local
BINDIR=${PREFIX}/bin
MAN1DIR=${PREFIX}/share/man/man1
MAN3DIR=${PREFIX}/share/man/man3

# Options
CC = cc
CFLAGS = -std=c99 -O -Wall -Wmissing-prototypes
#CFLAGS = -std=c99 -g -Wall -Wmissing-prototypes
#LFLAGS = -L/usr/local/lib -lefence
EXECS = geog
OBJ = geog_app.o geog_lib.o geog_proj.o alloc.o
all : ${EXECS}

obj : ${OBJ}

install : ${EXECS}
	mkdir -p ${BINDIR}
	cp ${EXECS} ${BINDIR}
	mkdir -p ${MAN1DIR}
	cp ../man/man1/*.1 ${MAN1DIR}
	mkdir -p ${MAN3DIR}
	cp ../man/man3/*.3 ${MAN3DIR}

geog : ${OBJ}
	${CC} ${CFLAGS} -o geog ${OBJ} -lm

geog_app.o : geog_app.c geog_lib.h
	${CC} ${CFLAGS} -c geog_app.c

cart_pol : cart_pol.c geog_lib.c alloc.c
	${CC} ${CFLAGS} -o cart_pol cart_pol.c geog_lib.c alloc.c -lm

geog_lib.o : geog_lib.c geog_lib.h
	${CC} ${CFLAGS} -c geog_lib.c

geog_proj.o : geog_proj.c geog_proj.h geog_lib.h
	${CC} ${CFLAGS} -c geog_proj.c

alloc.o : alloc.c alloc.h
	${CC} ${CFLAGS} -c alloc.c

clean :
	rm -fr ${EXECS} *core*  *.o *.ps *.out *.dSYM
