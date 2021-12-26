
#--------------------------------------------
SHELL	=	SH

#--------------------------------------------
CC       = ee-gcc
AS       = ee-gcc
LD       = ee-ld
CONV       = ./outpatch

#--------------------------------------------
#�\�[�X�̂���t�H���_
SOURCEDIR =  source/

#--------------------------------------------
# �v���W�F�N�g���i���������t�@�C�����j
NAME	=	sennin

#--------------------------------------------
# �v���W�F�N�g�Ŏg�p����A�Z���u���t�@�C��

#-- �ʏ펞
.SFILES	=	$(SOURCEDIR)startup.s

#--------------------------------------------
# �v���W�F�N�g�Ŏg�p����b����t�@�C��
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
# �I�u�W�F�N�g�t�@�C��
.OFILES	=	$(.CFILES:.c=.o)\
			$(.SFILES:.s=.o)


#--------------------------------------------
# �A�Z���u�����t���O
ASFLAGS	=	-march=r4000 -O3 -mgp32 -c -xassembler -O

#--------------------------------------------
# �R���p�C�����t���O
# �œK������ -O3
CFLAGS	= -march=r4000 -O3 -mgp32 -mlong32

#--------------------------------------------
# �����N���t���O

# -- �ʏ펞
LDFLAGS	 =-O0 -M -Ttext 8900000 -q -o out > $(NAME).map

#--------------------------------------------
# ���������t�@�C����
DEPENDFILE	=	Makedepend.txt
MAPFILE		=	$(NAME).map
TARGET_ELF	=	out
TARGET_BIN	=	 EBOOT.PBP


# �R���p�C���E�����N����ߒ�
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
