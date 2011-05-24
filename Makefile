CFLAGS = -g -Wall -O2 -std=c99

SOURCES = $(wildcard *.c)
OBJS = $(SOURCES:.c=.o)

all: dpa

dpa: $(OBJS) -lm

ifneq ($(MAKECMDGOALS),clean)
-include $(subst .c,.d,$(SOURCES))
endif

%.d : %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -MM -MF $@ -MP -MT $(subst .c,.o,$<) $<


.PHONY: clean

clean:
	rm -f *.o *.d dpa core
