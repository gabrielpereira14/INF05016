ARGS = -Wall -g -O0
SRCDIR = src
OBJDIR = obj
BINDIR = bin

hkObjects = $(addprefix $(OBJDIR)/, hopcroft-karp.o graph.o)
bvnObjects = $(addprefix $(OBJDIR)/, birkhoff-von-neumann.o graph.o)
ekObjects = $(addprefix $(OBJDIR)/, edmond-karp.o graph.o bfs.o)

CC = g++

all: hk  bvn ek;

ek:$(ekObjects) | $(BINDIR)
	$(CC)  $^ -o $(BINDIR)/edmond-karp $(ARGS)

bvn: $(bvnObjects) | $(BINDIR)
	$(CC)  $^ -o $(BINDIR)/birkhoff-von-neumann $(ARGS)

hk: $(hkObjects) | $(BINDIR)
	$(CC)  $^ -o $(BINDIR)/hopcroft-karp $(ARGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CC) -c $< $(ARGS) -o $@ 

$(OBJDIR):	
	mkdir $(OBJDIR)

$(BINDIR):	
	mkdir $(BINDIR)

clean:
	-rm -f $(OBJDIR)/*.o $(BINDIR)\hopcroft-karp $(BINDIR)\birkhoff-von-neumann $(BINDIR)\edmond-karp
	-del -f $(BINDIR)\hopcroft-karp.exe $(BINDIR)\birkhoff-von-neumann.exe $(OBJDIR)\*.o  $(BINDIR)\edmond-karp.exe
	