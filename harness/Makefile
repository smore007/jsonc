TESTDIR = tests
INPUTS = $(wildcard $(TESTDIR)/*-in.json)
OUTPUTS = $(patsubst $(TESTDIR)/%-in.json,$(TESTDIR)/%-out.txt,$(INPUTS))

xjson: json.c
	cc -o xjson json.c


test: xjson
	@for input in $(INPUTS); do \
		expected=$${input%-in.json}-out.txt; \
		run_output=$${input%-in.json}-run.txt; \
		./xjson < $$input > $$run_output; \
		if diff -u $$expected $$run_output; then \
			echo "$$input: PASS"; \
			rm $$run_output; \
		else \
			echo "$$input: FAIL"; \
		fi; \
	done
