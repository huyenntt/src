#!/usr/bin/env bash

################################################
# Since the GNU Autotools lack of              #
# C++ Bison support this script will call      #
# Bison for a given name, rename the           #
# generated files and their mutual inclusions. #
################################################

# needed tools
SED="/bin/sed"
BISON="/usr/bin/bison"

# name definitions
NAME="$1"
SRC="${NAME}.yy"

GEN_CC="${NAME}.tab.cc"
GEN_H="${NAME}.tab.hh"
GEN_LOC="location.hh"
GEN_POS="position.hh"
GEN_STACK="stack.hh"

TARGET_CC="${NAME}.cc"
TARGET_H="${NAME}.h"
TARGET_LOC="${NAME}-location.h"
TARGET_POS="${NAME}-position.h"
TARGET_STACK="${NAME}-stack.h"

# sed-call
CALL="${SED} -e s/${GEN_CC}/${TARGET_CC}/ -e s/${GEN_H}/${TARGET_H}/ -e s/${GEN_LOC}/${TARGET_LOC}/ -e s/${GEN_POS}/${TARGET_POS}/ -e s/${GEN_STACK}/${TARGET_STACK}/"

#############################
# actual script starts here #
#############################

# generate (temporary) files
${BISON} ${SRC}

# replace filenames
${CALL} ${GEN_CC} > ${TARGET_CC}
${CALL} ${GEN_H} > ${TARGET_H}
${CALL} ${GEN_LOC} > ${TARGET_LOC}
${CALL} ${GEN_POS} > ${TARGET_POS}
${CALL} ${GEN_STACK} > ${TARGET_STACK}

# fix lexer
${SED} -e "s/<\(FlexLexer.h\)>/\"\1\"/" ${NAME}-lexer.cc > ${NAME}-lexer.tmp
mv -f ${NAME}-lexer.tmp ${NAME}-lexer.cc

# remove temporary files
rm -f ${GEN_CC} ${GEN_H} ${GEN_LOC} ${GEN_POS} ${GEN_STACK}

