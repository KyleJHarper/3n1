CC     = gcc
CFLAGS += -Wall -Wextra -std=gnu99 -pthread

SRCDIR = src
BINDIR = bin

SOURCES := $(wildcard $(SRCDIR)/*.c)

# Targets
build:
	$(MAKE) 3n1

3n1:
	$(CC) $(CFLAGS) -O3 -o $(BINDIR)/3n1 \
		$(SRCDIR)/helpers.c      \
		$(SRCDIR)/error.c        \
		$(SRCDIR)/options.c      \
		$(SRCDIR)/3n1.c

clean:
	rm -f $(BINDIR)/*
	echo "Cleanup complete!"
