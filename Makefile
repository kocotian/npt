CC 	= g++
STD = c++17
SRC = npt.cpp config.h headers/*

npt: ${SRC}
	@${CC} $< -o $@ -std=${STD} && exit 0 || exit 1

install: npt
	@(mkdir -p ${DESTDIR}${PREFIX}/bin && cp -f npt ${DESTDIR}${PREFIX}/bin && chmod 755 ${DESTDIR}${PREFIX}/bin/npt) && exit 0 || exit 1
