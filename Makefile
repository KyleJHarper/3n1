CC     = g++
CFLAGS += -Wall -Wextra -std=c++20 -pthread -O2

SRCDIR = src
BINDIR = bin
EXPDIR = $(SRCDIR)/experiments

SOURCES := $(wildcard $(SRCDIR)/*.c)

# Targets
build:
	$(MAKE) 3n1

3n1:
	$(CC) $(CFLAGS) -o $(BINDIR)/3n1 \
		$(EXPDIR)/experiment_a1.cpp \
		$(SRCDIR)/3n1.cpp

clean:
	rm -f $(BINDIR)/*
	echo "Cleanup complete!"
