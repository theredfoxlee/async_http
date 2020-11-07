CC          ?= gcc
RM          := rm -rf
RMDIR       := rmdir
MKDIR       := mkdir -p

TARGET      := $(shell basename $(CURDIR))
SRCDIR      := src
OBJDIR      := obj

CFLAGS      := -Wall -Wextra -pedantic

SRC         := $(shell find $(SRCDIR) -type f -name '*.c')
OBJ         := $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SRC:.c=.o))
DEP         := $(OBJ:.o=.d)
EXE         := $(OBJDIR)/async_http_example

-include $(DEP)

.PHONY: all setup clean $(OBJDIR)

all: $(OBJDIR) $(EXE)

$(OBJDIR):
	$(MKDIR) $(OBJDIR)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -lcurl -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -MMD -MP -o $@ $<

clean:
	$(RM) $(OBJ) $(DEP) $(EXE)
	$(RM) $(OBJDIR)
