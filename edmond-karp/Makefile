ARGS = -Wall -g
SRCDIR = src
OBJDIR = obj
BINDIR = bin

objects = $(addprefix $(OBJDIR)/, edmond-karp.o graph.o bfs.o)
stats_objects = $(addprefix $(OBJDIR)/, edmond-karp-stats.o  graph.o bfs-stats.o)

CC = g++

ek: $(objects) | $(BINDIR)
	$(CC)  $^ -o $(BINDIR)/edmond-karp $(ARGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CC) -c $< $(ARGS) -o $@

$(OBJDIR):	
	mkdir $(OBJDIR)

$(BINDIR):	
	mkdir $(BINDIR)

stats: $(stats_objects)
	$(CC)  $^ -o $(BINDIR)/edmond-karp-stats $(ARGS)

clean:
	-rm -f $(OBJDIR)/*.o edmond-karp edmond-karp-stats
	-del -f $(BINDIR)\edmond-karp.exe $(BINDIR)\edmond-karp-stats.exe $(OBJDIR)\*.o 
	