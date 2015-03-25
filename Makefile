SRC_DIR = src

TEST_DIR = test

BUILD_SUBDIR = build
SRC_BUILD_DIR = ${SRC_DIR}/${BUILD_SUBDIR}
TEST_BUILD_DIR = ${TEST_DIR}/${BUILD_SUBDIR}

GTEST_DIR = vendor/gtest

# Flags passed to the preprocessor.
CPPFLAGS += -I$(GTEST_DIR)/include

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -std=c++11

TESTS = $(foreach sdir,$(TEST_DIR),$(basename $(notdir $(wildcard $(sdir)/*.cc))))

RM = rm -rf

all : clean target $(TESTS)

clean :
	$(RM) ${SRC_BUILD_DIR} ${TEST_BUILD_DIR}

target :
	mkdir -p ${SRC_BUILD_DIR} ${TEST_BUILD_DIR}

OBJS=$(subst .cc,.o, $(notdir $(wildcard $(SRC_DIR)/**/*.cc)))
TEST_OBJS=$(subst .cc,.o, $(notdir $(wildcard $(TEST_DIR)/*.cc)))
define make-test-goal
$1 : $(addprefix ${SRC_BUILD_DIR}/, $(OBJS)) $(addprefix ${TEST_BUILD_DIR}/, $(TEST_OBJS)) ${TEST_BUILD_DIR}/gtest_main.a 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $$? -o ${TEST_BUILD_DIR}/$$@
endef
	
$(foreach test,$(TESTS),$(eval $(call make-test-goal,$(test))))

${TEST_BUILD_DIR}/%.o : $(TEST_DIR)/%.cc $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
	
${SRC_BUILD_DIR}/%.o : $(SRC_DIR)/utils/%.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

########################################
#                                      #
# Builds googletest framework binaries #
#                                      #
########################################

GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)


gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc -o ${TEST_BUILD_DIR}/$@

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc -o ${TEST_BUILD_DIR}/$@

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) ${TEST_BUILD_DIR}/$@ ${TEST_BUILD_DIR}/$^

${TEST_BUILD_DIR}/gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) ${TEST_BUILD_DIR}/gtest_main.a ${TEST_BUILD_DIR}/gtest-all.o ${TEST_BUILD_DIR}/gtest_main.o
