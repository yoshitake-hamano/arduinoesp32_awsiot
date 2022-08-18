SRC_DIR:=src
BUILD_DIR:=images
TARGETS:=$(BUILD_DIR)/summary.png

all: $(TARGETS)

.PHONY: clean
clean:
	$(RM) $(TARGETS)

$(BUILD_DIR)/%.png: $(SRC_DIR)/%.puml
	plantuml -Tpng -o ../$(BUILD_DIR) $^

