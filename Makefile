
build-dir=.

local_src := $(wildcard $(build-dir)/*.c)
local_obj := $(patsubst %c,%o,$(notdir $(local_src)))

.PHONY:all
all:
	@echo $(local_obj) $(local_src) $(notdir $(local_src))
