# この makefile は、msys や cygwin などの Unix 互換環境上で利用することを想定している。
# ビルドには xdev68k が必要。
# https://github.com/yosshin4004/xdev68k

# 必要な環境変数が定義されていることを確認する。
ifndef XDEV68K_DIR
	$(error ERROR : XDEV68K_DIR is not defined.)
endif

# デフォルトサフィックスを削除
.SUFFIXES:

# ビルド対象の CPU
CPU = 68000

# 各種コマンド短縮名
ATOMIC = perl ${XDEV68K_DIR}/util/atomic.pl
CXX = ${XDEV68K_DIR}/m68k-toolchain/bin/m68k-elf-g++
CC = ${XDEV68K_DIR}/m68k-toolchain/bin/m68k-elf-gcc
GAS2HAS = perl ../xdev68k/x68k_gas2has.pl -cpu $(CPU) -inc doscall.inc -inc iocscall.inc
# CNVTBL = gawk -f util/converttable.awk
RUN68 = $(ATOMIC) ${XDEV68K_DIR}/run68/run68
HAS = $(RUN68) ${XDEV68K_DIR}/x68k_bin/HAS060.X
HLK = $(RUN68) ${XDEV68K_DIR}/x68k_bin/hlk301.x
RUSTC = rustc -O -C strip=symbols --target wasm32-wasi
ZIG = zig build-exe -O ReleaseSmall -target wasm32-wasi-musl
WASM2C = ../BIN/wasm2c

# 最終生成物
EXECUTABLE = RSHELLO.X
TARGET_FILES = $(EXECUTABLE)

# ヘッダ検索パス
INCLUDE_FLAGS = -I${XDEV68K_DIR}/include/xc -I${XDEV68K_DIR}/include/xdev68k


# コンパイルフラグ
COMMON_FLAGS = -m$(CPU) -Os $(INCLUDE_FLAGS)
CFLAGS = $(COMMON_FLAGS) -D CLOCKS_PER_SEC=10000000 -Wno-builtin-declaration-mismatch -fcall-used-d2 -fcall-used-a2 -finput-charset=cp932 -fexec-charset=cp932 -fverbose-asm
CXXFLAGS = $(CFLAGS) -fno-rtti -fno-exceptions -std=gnu++17

# *.rsソースファイル
RS_SRCS = hello.rs

# *.zig ソースファイル
ZIG_SRCS =

# *.c ソースファイル
C_SRCS = lib/wasi.c lib/math.c lib/rmemcpy.c

# *.cpp ソースファイル
CPP_SRCS = 

# *.s ソースファイル
ASM_SRCS =

# リンク対象のライブラリファイル
LIBS =\
	${XDEV68K_DIR}/lib/xc/BASLIB.L \
	${XDEV68K_DIR}/lib/xc/CLIB.L \
	${XDEV68K_DIR}/lib/xc/DOSLIB.L \
	${XDEV68K_DIR}/lib/xc/IOCSLIB.L \
	${XDEV68K_DIR}/lib/xc/FLOATFNC.L \
	${XDEV68K_DIR}/lib/m68k_elf/m$(CPU)/libgcc.a \
	${XDEV68K_DIR}/lib/m68k_elf/m$(CPU)/libstdc++.a \

# 中間ファイル生成用ディレクトリ
INTERMEDIATE_DIR = _build/m$(CPU)

# オブジェクトファイル
OBJS =	$(addprefix $(INTERMEDIATE_DIR)/,$(patsubst %.c,%.o,$(C_SRCS))) \
	$(addprefix $(INTERMEDIATE_DIR)/,$(patsubst %.cpp,%.o,$(CPP_SRCS))) \
	$(addprefix $(INTERMEDIATE_DIR)/,$(patsubst %.s,%.o,$(ASM_SRCS))) \
	$(addprefix $(INTERMEDIATE_DIR)/,$(patsubst %.rs,%.o,$(RS_SRCS))) \
	$(addprefix $(INTERMEDIATE_DIR)/,$(patsubst %.zig,%.o,$(ZIG_SRCS))) \

# 依存関係ファイル
DEPS =	$(addprefix $(INTERMEDIATE_DIR)/,$(patsubst %.c,%.d,$(C_SRCS))) \
	$(addprefix $(INTERMEDIATE_DIR)/,$(patsubst %.cpp,%.d,$(CPP_SRCS)))

# HLK に入力するリンクリスト
HLK_LINK_LIST = $(INTERMEDIATE_DIR)/_lk_list.tmp

# デフォルトのターゲット
all : $(TARGET_FILES)

# 依存関係ファイルの include
-include $(DEPS)

# 中間生成物の削除
clean : 
	rm -f $(TARGET_FILES)
	rm -rf $(INTERMEDIATE_DIR)

# 実行ファイルの生成
#	HLK に長いパス文字を与えることは難しい。
#	回避策としてリンク対象ファイルを $(INTERMEDIATE_DIR) 以下にコピーし、
#	短い相対パスを用いてリンクを実行させる。
$(EXECUTABLE) : $(OBJS) $(LIBS)
	mkdir -p $(INTERMEDIATE_DIR)
	rm -f $(HLK_LINK_LIST)
	@for FILENAME in $(OBJS); do\
		echo $$FILENAME >> $(HLK_LINK_LIST); \
	done
	@for FILENAME in $(LIBS); do\
		cp $$FILENAME $(INTERMEDIATE_DIR)/`basename $$FILENAME`; \
		echo $(INTERMEDIATE_DIR)/`basename $$FILENAME` >> $(HLK_LINK_LIST); \
	done
	$(HLK) -d _HEAP_SIZE=140000 -p -i $(HLK_LINK_LIST) -o $(EXECUTABLE)

# *.c ソースのコンパイル
$(INTERMEDIATE_DIR)/%.o : %.c makefile
	mkdir -p $(dir $(INTERMEDIATE_DIR)/$*.o)
	$(CC) -S $(CFLAGS) -o $(INTERMEDIATE_DIR)/$*.m68k-gas.s -MT $(INTERMEDIATE_DIR)/$*.o -MD -MP -MF $(INTERMEDIATE_DIR)/$*.d $<
	$(GAS2HAS) -i $(INTERMEDIATE_DIR)/$*.m68k-gas.s -o $(INTERMEDIATE_DIR)/$*.s
#	$(CNVTBL) < $(INTERMEDIATE_DIR)/$*.before-table.s > $(INTERMEDIATE_DIR)/$*.s
	$(HAS) -e -u -w0 $(INCLUDE_FLAGS) $(INTERMEDIATE_DIR)/$*.s -o $(INTERMEDIATE_DIR)/$*.o
#	rm -f $(INTERMEDIATE_DIR)/$*.m68k-gas.s

# *.cpp ソースのコンパイル
$(INTERMEDIATE_DIR)/%.o : %.cpp makefile
	mkdir -p $(dir $(INTERMEDIATE_DIR)/$*.o)
	$(CXX) -S $(CXXFLAGS) -o $(INTERMEDIATE_DIR)/$*.m68k-gas.s -MT $(INTERMEDIATE_DIR)/$*.o -MD -MP -MF $(INTERMEDIATE_DIR)/$*.d $<
	$(GAS2HAS) -i $(INTERMEDIATE_DIR)/$*.m68k-gas.s -o $(INTERMEDIATE_DIR)/$*.s
#	$(CNVTBL) < $(INTERMEDIATE_DIR)/$*.before-table.s > $(INTERMEDIATE_DIR)/$*.s
	$(HAS) -e -u -w0 $(INCLUDE_FLAGS) $(INTERMEDIATE_DIR)/$*.s -o $(INTERMEDIATE_DIR)/$*.o
#	rm -f $(INTERMEDIATE_DIR)/$*.m68k-gas.s

# *.s ソースのアセンブル
$(INTERMEDIATE_DIR)/%.o : %.s makefile
	mkdir -p $(dir $(INTERMEDIATE_DIR)/$*.o)
	$(HAS) -e -u -w0 $(INCLUDE_FLAGS) $*.s -o $(INTERMEDIATE_DIR)/$*.o

# *.rs ソースのコンパイル
%.c : %.rs makefile
	$(RUSTC) $<
	$(WASM2C) $*.wasm $*.c

# *.zig ソースのコンパイル
%.c : %.zig makefile
	$(ZIG) $<
	$(WASM2C) $*.wasm $*.c
