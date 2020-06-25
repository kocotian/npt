SRC = npt.cpp

make:
	@echo "Compiling npt 0.1..."
	@g++ npt.cpp -o npt -std=c++17 && echo "Compilation finished successfully" || echo "Compilation failed"

install:
	@echo Installing npt 2.0 to ${DESTDIR}${PREFIX}/bin... && mkdir -p ${DESTDIR}${PREFIX}/bin && cp -f npt ${DESTDIR}${PREFIX}/bin && chmod 755 ${DESTDIR}${PREFIX}/bin/npt && echo "Installation finished successfully" || echo "Something gone wrong..."
