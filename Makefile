
#--------------------------------------------
SHELL	=	SH

#--------------------------------------------
CC       = ee-gcc
AS       = ee-gcc
LD       = ee-ld
CONV       = ./outpatch

#--------------------------------------------
#ソースのあるフォルダ
SOURCEDIR =  source/

#--------------------------------------------
# プロジェクト名（生成されるファイル名）
NAME	=	sennin

#--------------------------------------------
# プロジェクトで使用するアセンブラファイル

#-- 通常時
.SFILES	=	$(SOURCEDIR)startup.s

#--------------------------------------------
# プロジェクトで使用するＣ言語ファイル
.CFILES	=	$(SOURCEDIR)main.c 			\
			$(SOURCEDIR)pg.c	 		\
			$(SOURCEDIR)font.c 			\
			$(SOURCEDIR)utility.c 		\
			$(SOURCEDIR)_clib.c 		\
			$(SOURCEDIR)game.c 			\
			$(SOURCEDIR)enemy.c 		\
			$(SOURCEDIR)sennindata.c 	\
			$(SOURCEDIR)sound.c


#--------------------------------------------
# オブジェクトファイル
.OFILES	=	$(.CFILES:.c=.o)\
			$(.SFILES:.s=.o)


#--------------------------------------------
# アセンブル時フラグ
ASFLAGS	=	-march=r4000 -O3 -mgp32 -c -xassembler -O

#--------------------------------------------
# コンパイル時フラグ
# 最適化あり -O3
CFLAGS	= -march=r4000 -O3 -mgp32 -mlong32

#--------------------------------------------
# リンク時フラグ

# -- 通常時
LDFLAGS	 =-O0 -M -Ttext 8900000 -q -o out > $(NAME).map

#--------------------------------------------
# 生成されるファイル名
DEPENDFILE	=	Makedepend.txt
MAPFILE		=	$(NAME).map
TARGET_ELF	=	out
TARGET_BIN	=	 EBOOT.PBP


# コンパイル・リンクする過程
#$(TARGET_BIN): $(TARGET_ELF)
#	objcopy -v -O binary $< $@

#all:	 EBOOT.PBP
all: clean depend data $(TARGET_BIN)

outp	: out
	$(CONV)
	-rm out

EBOOT.PBP : outp
	elf2pbp outp "Sennin Game"
	-rm outp



out : $(.OFILES) Makefile $(.AFILES) $(DEPENDFILE)
#	@echo > $(MAPFILE)
	$(LD) -g -o $(TARGET_ELF) $(.OFILES) $(LDFLAGS)


.PHONY: all clean depend data


clean:
	-rm  $(DEPENDFILE) $(.OFILES)

clean_data:
	$(foreach .ADIR_TMP, $(dir $(.AFILES)), make -C $(.ADIR_TMP) clean;)

depend:
	$(CC) $(CFLAGS) -M $(.CFILES) > $(DEPENDFILE)

$(DEPENDFILE): 
	$(CC) $(CFLAGS) -M $(.CFILES) > $(DEPENDFILE)

.SUFFIXES: .s .c .o .a
