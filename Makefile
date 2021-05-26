## -*- Makefile -*-

#### Compiler and tool definitions shared by all build targets #####
CCC = g++
CXX = g++
BASICOPTS = -O -m64
CCFLAGS = $(BASICOPTS)
CXXFLAGS = $(BASICOPTS)
CCADMIN = 


# Define the target directories.
TARGETDIR_streamfiler=dist


all: $(TARGETDIR_streamfiler)/streamfiler

## Target: streamfiler
OBJS_streamfiler =  \
	$(TARGETDIR_streamfiler)/CStreamFilerCentral.o \
	$(TARGETDIR_streamfiler)/CStreamFilerFileHandler.o \
	$(TARGETDIR_streamfiler)/CStreamFilerHistogramMaker.o \
	$(TARGETDIR_streamfiler)/CStreamFilerTcpReaderThread.o \
	$(TARGETDIR_streamfiler)/CStreamFilerTcpReader.o \
	$(TARGETDIR_streamfiler)/CStreamFilerTimeData.o \
	$(TARGETDIR_streamfiler)/streamfiler.o
SYSLIBS_streamfiler = -lpthread 
USERLIBS_streamfiler = $(SYSLIBS_streamfiler) 
DEPLIBS_streamfiler =  
LDLIBS_streamfiler = $(USERLIBS_streamfiler)


# Link or archive
$(TARGETDIR_streamfiler)/streamfiler: $(TARGETDIR_streamfiler) $(OBJS_streamfiler) $(DEPLIBS_streamfiler)
	$(LINK.cc) $(CCFLAGS_streamfiler) $(CPPFLAGS_streamfiler) -o $@ $(OBJS_streamfiler) $(LDLIBS_streamfiler)


# Compile source files into .o files
$(TARGETDIR_streamfiler)/CStreamFilerCentral.o: $(TARGETDIR_streamfiler) src/CStreamFilerCentral.cpp
	$(COMPILE.cc) $(CCFLAGS_streamfiler) $(CPPFLAGS_streamfiler) -o $@ src/CStreamFilerCentral.cpp

$(TARGETDIR_streamfiler)/CStreamFilerFileHandler.o: $(TARGETDIR_streamfiler) src/CStreamFilerFileHandler.cpp
	$(COMPILE.cc) $(CCFLAGS_streamfiler) $(CPPFLAGS_streamfiler) -o $@ src/CStreamFilerFileHandler.cpp

$(TARGETDIR_streamfiler)/CStreamFilerHistogramMaker.o: $(TARGETDIR_streamfiler) src/CStreamFilerHistogramMaker.cpp
	$(COMPILE.cc) $(CCFLAGS_streamfiler) $(CPPFLAGS_streamfiler) -o $@ src/CStreamFilerHistogramMaker.cpp

$(TARGETDIR_streamfiler)/CStreamFilerTcpReaderThread.o: $(TARGETDIR_streamfiler) src/CStreamFilerTcpReaderThread.cpp
	$(COMPILE.cc) $(CCFLAGS_streamfiler) $(CPPFLAGS_streamfiler) -o $@ src/CStreamFilerTcpReaderThread.cpp

$(TARGETDIR_streamfiler)/CStreamFilerTcpReader.o: $(TARGETDIR_streamfiler) src/CStreamFilerTcpReader.cpp
	$(COMPILE.cc) $(CCFLAGS_streamfiler) $(CPPFLAGS_streamfiler) -o $@ src/CStreamFilerTcpReader.cpp

$(TARGETDIR_streamfiler)/CStreamFilerTimeData.o: $(TARGETDIR_streamfiler) src/CStreamFilerTimeData.cpp
	$(COMPILE.cc) $(CCFLAGS_streamfiler) $(CPPFLAGS_streamfiler) -o $@ src/CStreamFilerTimeData.cpp

$(TARGETDIR_streamfiler)/streamfiler.o: $(TARGETDIR_streamfiler) src/streamfiler.cpp
	$(COMPILE.cc) $(CCFLAGS_streamfiler) $(CPPFLAGS_streamfiler) -o $@ src/streamfiler.cpp



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR_streamfiler)/streamfiler \
		$(TARGETDIR_streamfiler)/CStreamFilerCentral.o \
		$(TARGETDIR_streamfiler)/CStreamFilerFileHandler.o \
		$(TARGETDIR_streamfiler)/CStreamFilerHistogramMaker.o \
		$(TARGETDIR_streamfiler)/CStreamFilerTcpReaderThread.o \
		$(TARGETDIR_streamfiler)/CStreamFilerTcpReader.o \
		$(TARGETDIR_streamfiler)/CStreamFilerTimeData.o \
		$(TARGETDIR_streamfiler)/streamfiler.o
	$(CCADMIN)
	rm -f -r $(TARGETDIR_streamfiler)


# Create the target directory (if needed)
$(TARGETDIR_streamfiler):
	mkdir -p $(TARGETDIR_streamfiler)


# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-amd64-Linux

