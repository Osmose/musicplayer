UADECOREDIR = /usr/lib/uade
UADECORENAME=uadecore
PKGCONFIGDIR = /usr/lib/pkgconfig

CC = gcc
NATIVECC = gcc
ARCHFLAGS = 
ARCHLIBS =  -L/usr/local/lib -Wl,-rpath,/usr/local/lib
DEBUGFLAGS = -g

COMMONGCCOPTS = -Wall -Wno-unused -Wno-format -Wmissing-prototypes -Wstrict-prototypes -fno-exceptions -O2

TARGETCFLAGS = -fomit-frame-pointer $(COMMONGCCOPTS) $(DEBUGFLAGS) $(ARCHFLAGS)
LIBRARIES = -lm $(AUDIOLIBS) $(ARCHLIBS)

# Native flags are used to build tools that generate new code that is then
# compiled with the target compiler.
NATIVECFLAGS = $(COMMONGCCOPTS) $(DEBUGFLAGS)

.SUFFIXES: .a .o .c .h .S

.SECONDARY: cpuemu.c cpustbl.c cputbl.h

INCLUDES=-I. -I./include -Ifrontends/include

CPUEMUOBJS = cpuemu1.o cpuemu2.o cpuemu3.o cpuemu4.o cpuemu5.o cpuemu6.o cpuemu7.o cpuemu8.o

OBJS = main.o newcpu.o memory.o custom.o cia.o audio.o compiler.o cpustbl.o \
       missing.o sd-sound.o md-support.o cfgfile.o fpp.o debug.o \
       readcpu.o cpudefs.o $(CPUEMUOBJS) \
       uade.o uadeipc.o uadeutils.o unixatomic.o ossupport.o \
       uademain.o sinctable.o text_scope.o

all:	uadecore

uadecore:	$(OBJS)
	$(CC) $(ARCHFLAGS) -o $@ $(OBJS) $(LIBRARIES)

clean:
	-rm -f $(OBJS) *.o uadecore
	-rm -f gencpu cpudefs.c uadeipc.c
	-rm -f cpuemu.c build68k cputmp.s cpustbl.c cputbl.h

install:	$(UADECORENAME)
	mkdir -m 755 -p "$(UADECOREDIR)" "$(PKGCONFIGDIR)"
	install "$(UADECORENAME)" "$(UADECOREDIR)"/

# First native builds

build68k: build68k.o
	$(NATIVECC) $(NATIVECFLAGS) $(INCLUDES) -o $@ build68k.o

gencpu:	gencpu.o nativereadcpu.o nativecpudefs.o nativemissing.o
	$(NATIVECC) $(NATIVECFLAGS) $(INCLUDES) -o $@ gencpu.o nativereadcpu.o nativecpudefs.o nativemissing.o

build68k.o:	build68k.c include/readcpu.h
	$(NATIVECC) $(NATIVECFLAGS) $(INCLUDES) -c $<

gencpu.o:	gencpu.c include/readcpu.h
	$(NATIVECC) $(NATIVECFLAGS) $(INCLUDES) -c $<

nativereadcpu.o:	readcpu.c include/readcpu.h
	$(NATIVECC) $(NATIVECFLAGS) $(INCLUDES) -c -o $@ $<

nativemissing.o:	missing.c
	$(NATIVECC) $(NATIVECFLAGS) $(INCLUDES) -c -o $@ $<

cpuemu.c cputbl.h cpustbl.c: gencpu
	./gencpu

nativecpudefs.o:	cpudefs.c include/readcpu.h
	$(NATIVECC) $(NATIVECFLAGS) $(INCLUDES) -c -o $@ $<

cpudefs.c:	build68k table68k
	./build68k <./table68k >cpudefs.c


# Then target builds

cpustbl.o: cpuemu.c
	$(CC) $(INCLUDES) -c $(INCDIRS) $(TARGETCFLAGS)  -o $@ cpustbl.c

cpuemu1.o: cpuemu.c
	$(CC) -DPART_1 $(INCLUDES) -c $(INCDIRS) $(TARGETCFLAGS)  -o $@ $<

cpuemu2.o: cpuemu.c
	$(CC) -DPART_2 $(INCLUDES) -c $(INCDIRS) $(TARGETCFLAGS)  -o $@ $<

cpuemu3.o: cpuemu.c
	$(CC) -DPART_3 $(INCLUDES) -c $(INCDIRS) $(TARGETCFLAGS)  -o $@ $<

cpuemu4.o: cpuemu.c
	$(CC) -DPART_4 $(INCLUDES) -c $(INCDIRS) $(TARGETCFLAGS)  -o $@ $<

cpuemu5.o: cpuemu.c
	$(CC) -DPART_5 $(INCLUDES) -c $(INCDIRS) $(TARGETCFLAGS)  -o $@ $<

cpuemu6.o: cpuemu.c
	$(CC) -DPART_6 $(INCLUDES) -c $(INCDIRS) $(TARGETCFLAGS)  -o $@ $<

cpuemu7.o: cpuemu.c
	$(CC) -DPART_7 $(INCLUDES) -c $(INCDIRS) $(TARGETCFLAGS)  -o $@ $<

cpuemu8.o: cpuemu.c
	$(CC) -DPART_8 $(INCLUDES) -c $(INCDIRS) $(TARGETCFLAGS)  -Wno-sign-compare -o $@ $<

custom.o:	include/audio.h

.rc.res:
	$(WRC) $(INCLUDES) $<
.m.o:
	$(CC) $(INCLUDES) -c $(INCDIRS) $(TARGETCFLAGS)  $<
.c.o:
	$(CC) $(INCLUDES) -c $(INCDIRS) $(TARGETCFLAGS)  $< -o $@
.c.s:
	$(CC) $(INCLUDES) -S $(INCDIRS) $(TARGETCFLAGS)  $< -o $@
.c.i:
	$(CC) $(INCLUDES) -E $(INCDIRS) $(TARGETCFLAGS)  $< > $@
.S.o:
	$(CC) $(INCLUDES) -c $(INCDIRS) $(TARGETCFLAGS)  $< -o $@
.s.o:
	$(CC) $(INCLUDES) -c $(INCDIRS) $(TARGETCFLAGS)  $< -o $@

# Saves recompiling...
touch:	
	touch *.o; touch build68k; touch cpudefs.c; touch cpudefs.o; touch gencpu; touch cpuemu.c; touch cpuemu*.o cpufast.o

cpudefs.o:	cpudefs.c include/readcpu.h
readcpu.o:	readcpu.c include/readcpu.h

main.o:	include/uae.h
cia.o: include/events.h
custom.o: include/events.h
newcpu.o: newcpu.c include/uadectl.h include/events.h frontends/include/uade/uadeipc.h
	$(CC) $(INCLUDES) -c $(INCDIRS) $(TARGETCFLAGS)  newcpu.c

sd-sound.o:	include/uadectl.h sd-sound.c sd-sound.h frontends/include/uade/uadeconstants.h sd-sound-generic.h sd-sound-generic.c
audio.o: include/uadectl.h include/events.h sd-sound.h include/gensound.h include/audio.h frontends/include/uade/uadeconstants.h include/sinctable.h include/text_scope.h sd-sound-generic.h
sinctable.o:	include/sinctable.h
memory.o:
debug.o: 
fpp.o: 

uademain.o:	uademain.c include/uae.h frontends/include/uade/ossupport.h frontends/include/uade/unixsupport.h

uade.o:	uade.c include/uadectl.h sd-sound.h frontends/include/uade/uadeipc.h frontends/include/uade/uadeconstants.h frontends/include/uade/ossupport.h frontends/include/uade/unixsupport.h include/amigamsg.h frontends/include/uade/sysincludes.h

uadeipc.o:	uadeipc.c

uadeipc.c:	frontends/common/uadeipc.c frontends/include/uade/uadeipc.h frontends/include/uade/ossupport.h frontends/include/uade/unixsupport.h frontends/include/uade/sysincludes.h
	cp $< .

ossupport.c:	frontends/common/ossupport.c
	cp $< .

ossupport.o:	ossupport.c frontends/include/uade/ossupport.h frontends/include/uade/unixsupport.h include/uadectl.h frontends/include/uade/unixatomic.h unixatomic.c

unixatomic.c:	frontends/common/unixatomic.c
	cp $< .

unixatomic.o:	unixatomic.c frontends/include/uade/sysincludes.h frontends/include/uade/unixatomic.h

uadeutils.c:	frontends/common/uadeutils.c
	cp $< .

uadeutils.o:	uadeutils.c frontends/include/uade/uadeutils.h

textscope.o:	text_scope.c include/text_scope.h include/custom.h
