#-------------------------------------------------
#
# Project created by QtCreator 2017-06-28T17:59:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += \
	../Kreatures/libFreetures/src \
	../Kreatures/libFreetures/include \
	../squish-1.11/ \
	src/

TARGET = PoseGaitEditor
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    ../Kreatures/libFreetures/src/body/pose_string_table.cpp \
    ../Kreatures/libFreetures/src/catalogue/catalogue_hash.cpp \
    ../Kreatures/libFreetures/src/biochemistry/emitter.c \
    ../Kreatures/libFreetures/src/biochemistry/organ.c \
    ../Kreatures/libFreetures/src/biochemistry/reaction.c \
    ../Kreatures/libFreetures/src/biochemistry/receptor.c \
    ../Kreatures/libFreetures/src/body/appearance.c \
    ../Kreatures/libFreetures/src/body/default_poses.c \
    ../Kreatures/libFreetures/src/body/expression.c \
    ../Kreatures/libFreetures/src/body/pigment.c \
    ../Kreatures/libFreetures/src/body/pose.c \
    ../Kreatures/libFreetures/src/body/skell.c \
    ../Kreatures/libFreetures/src/brain/brain_organ.c \
    ../Kreatures/libFreetures/src/brain/load_lobe.c \
    ../Kreatures/libFreetures/src/brain/load_temporal.c \
    ../Kreatures/libFreetures/src/brain/load_tract.c \
    ../Kreatures/libFreetures/src/brain/lobe.c \
    ../Kreatures/libFreetures/src/brain/temporal.c \
    ../Kreatures/libFreetures/src/brain/tract.c \
    ../Kreatures/libFreetures/src/catalogue/catalogue.c \
    ../Kreatures/libFreetures/src/catalogue/catalogue_array.c \
    ../Kreatures/libFreetures/src/catalogue/catalogue_array_range.c \
    ../Kreatures/libFreetures/src/catalogue/catalogue_file_range.c \
    ../Kreatures/libFreetures/src/catalogue/catalogue_loader.c \
    ../Kreatures/libFreetures/src/catalogue/genome_symbols.c \
    ../Kreatures/libFreetures/src/creature/create_creature.c \
    ../Kreatures/libFreetures/src/creature/creature.c \
    ../Kreatures/libFreetures/src/genetics/chromosomerange.c \
    ../Kreatures/libFreetures/src/genetics/createmoniker.c \
    ../Kreatures/libFreetures/src/genetics/export_genome.c \
    ../Kreatures/libFreetures/src/genetics/gen_range.c \
    ../Kreatures/libFreetures/src/genetics/genemarkup.c \
    ../Kreatures/libFreetures/src/genetics/genetichandle.c \
    ../Kreatures/libFreetures/src/genetics/import_genes.c \
    ../Kreatures/libFreetures/src/genetics/import_genome.c \
    ../Kreatures/libFreetures/src/genetics/import_support_flex.c \
    ../Kreatures/libFreetures/src/genetics/load_genome.c \
    ../Kreatures/libFreetures/src/genetics/mergegenome.c \
    ../Kreatures/libFreetures/src/genetics/mutate.c \
    ../Kreatures/libFreetures/src/genetics/sanatizegenome.c \
    ../Kreatures/libFreetures/src/neurochemistry/neuro_conn.c \
    ../Kreatures/libFreetures/src/neurochemistry/neuro_emitter.c \
    ../Kreatures/libFreetures/src/neurochemistry/neuro_receptor.c \
    ../Kreatures/libFreetures/src/parser/grammar.c \
    ../Kreatures/libFreetures/src/parser/lexical.c \
    ../Kreatures/libFreetures/src/parser/markupwriter.c \
    ../Kreatures/libFreetures/src/parser/strdup.c \
    ../Kreatures/libFreetures/src/parser/tokenizer.c \
    ../Kreatures/libFreetures/src/parser/tokentypes.c \
    ../Kreatures/libFreetures/src/ranges/directory_range.c \
    ../Kreatures/libFreetures/src/svrules/sv_interpreter.c \
    ../squish-1.11/alpha.cpp \
    ../squish-1.11/clusterfit.cpp \
    ../squish-1.11/colourblock.cpp \
    ../squish-1.11/colourfit.cpp \
    ../squish-1.11/colourset.cpp \
    ../squish-1.11/maths.cpp \
    ../squish-1.11/rangefit.cpp \
    ../squish-1.11/singlecolourfit.cpp \
    ../squish-1.11/squish.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/bodydata.cpp \
    src/sprite.cpp \
    src/spritemanager.cpp \
    src/directoryrange.cpp \
    src/viewwidget.cpp \
    src/spriteinstance.cpp \
    src/qt_skell.cpp

HEADERS += \
    ../Kreatures/libFreetures/include/brain.h \
    ../Kreatures/libFreetures/include/catalogue.h \
    ../Kreatures/libFreetures/include/creature.h \
    ../Kreatures/libFreetures/include/definitions.h \
    ../Kreatures/libFreetures/include/genetics.h \
    ../Kreatures/libFreetures/include/gui_adapter.h \
    ../Kreatures/libFreetures/include/skell.h \
    ../Kreatures/libFreetures/include/types.h \
    ../Kreatures/libFreetures/src/behavior/behavior.h \
    ../Kreatures/libFreetures/src/behavior/instinct.h \
    ../Kreatures/libFreetures/src/behavior/stimulus.h \
    ../Kreatures/libFreetures/src/biochemistry/biochemistry.h \
    ../Kreatures/libFreetures/src/biochemistry/emitter.h \
    ../Kreatures/libFreetures/src/biochemistry/half_lives.h \
    ../Kreatures/libFreetures/src/biochemistry/organ.h \
    ../Kreatures/libFreetures/src/biochemistry/reaction.h \
    ../Kreatures/libFreetures/src/biochemistry/receptor.h \
    ../Kreatures/libFreetures/src/biochemistry/sources.h \
    ../Kreatures/libFreetures/src/biochemistry/waypoint.h \
    ../Kreatures/libFreetures/src/body/appearance.h \
    ../Kreatures/libFreetures/src/body/body.h \
    ../Kreatures/libFreetures/src/body/expression.h \
    ../Kreatures/libFreetures/src/body/pigment.h \
    ../Kreatures/libFreetures/src/body/pose_gait.h \
    ../Kreatures/libFreetures/src/body/pose_string_table.h \
    ../Kreatures/libFreetures/src/body/skell.h \
    ../Kreatures/libFreetures/src/brain/brain.h \
    ../Kreatures/libFreetures/src/brain/brain_organ.h \
    ../Kreatures/libFreetures/src/brain/lobe.h \
    ../Kreatures/libFreetures/src/brain/temporal.h \
    ../Kreatures/libFreetures/src/brain/tract.h \
    ../Kreatures/libFreetures/src/brain/waypoint.h \
    ../Kreatures/libFreetures/src/catalogue/catalogue.h \
    ../Kreatures/libFreetures/src/catalogue/catalogue_array.h \
    ../Kreatures/libFreetures/src/catalogue/catalogue_array_range.h \
    ../Kreatures/libFreetures/src/catalogue/catalogue_file_range.h \
    ../Kreatures/libFreetures/src/catalogue/catalogue_interface.h \
    ../Kreatures/libFreetures/src/catalogue/catalogue_loader.h \
    ../Kreatures/libFreetures/src/creature/creature.h \
    ../Kreatures/libFreetures/src/creature/genetic_creature.h \
    ../Kreatures/libFreetures/src/genetics/chromosomerange.h \
    ../Kreatures/libFreetures/src/genetics/gen_range.h \
    ../Kreatures/libFreetures/src/genetics/gene_header.h \
    ../Kreatures/libFreetures/src/genetics/genes.h \
    ../Kreatures/libFreetures/src/genetics/genetichandle.h \
    ../Kreatures/libFreetures/src/genetics/genome.h \
    ../Kreatures/libFreetures/src/genetics/genome_symbols.h \
    ../Kreatures/libFreetures/src/genetics/import_support_flex.h \
    ../Kreatures/libFreetures/src/genetics/sanatizegenome.h \
    ../Kreatures/libFreetures/src/neurochemistry/neuro_conn.h \
    ../Kreatures/libFreetures/src/neurochemistry/neuro_emitter.h \
    ../Kreatures/libFreetures/src/neurochemistry/neuro_receptor.h \
    ../Kreatures/libFreetures/src/neurochemistry/neurochemistry.h \
    ../Kreatures/libFreetures/src/parser/lexer.h \
    ../Kreatures/libFreetures/src/parser/lexical.h \
    ../Kreatures/libFreetures/src/parser/strdup.h \
    ../Kreatures/libFreetures/src/parser/tokenizer.h \
    ../Kreatures/libFreetures/src/parser/tokentypes.h \
    ../Kreatures/libFreetures/src/ranges/directory_range.h \
    ../Kreatures/libFreetures/src/svrules/sv_opcodes.h \
    ../squish-1.11/alpha.h \
    ../squish-1.11/clusterfit.h \
    ../squish-1.11/colourblock.h \
    ../squish-1.11/colourfit.h \
    ../squish-1.11/colourset.h \
    ../squish-1.11/config.h \
    ../squish-1.11/maths.h \
    ../squish-1.11/rangefit.h \
    ../squish-1.11/simd.h \
    ../squish-1.11/simd_float.h \
    ../squish-1.11/simd_sse.h \
    ../squish-1.11/simd_ve.h \
    ../squish-1.11/singlecolourfit.h \
    ../squish-1.11/squish.h \
    src/mainwindow.h \
    src/bodydata.h \
    src/byte_swap.h \
    sprite.h \
    src/src/spritemanager.h \
    src/directoryrange.h \
    src/viewwidget.h \
    src/spriteinstance.h \
    src/qt_skell.h

FORMS += \
        src/mainwindow.ui

