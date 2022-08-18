SRC_DIR:=src
BUILD_DIR:=images
TARGETS:=$(BUILD_DIR)/summary.svg

all: $(TARGETS)

.PHONY: clean
clean:
	$(RM) $(TARGETS)

$(BUILD_DIR)/%.svg: $(SRC_DIR)/%.puml
	plantuml -Tsvg -o ../$(BUILD_DIR) $^

