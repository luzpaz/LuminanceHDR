/**
 * This file is a part of Luminance HDR package.
 * ----------------------------------------------------------------------
 * Copyright (C) 2011 Davide Anastasia
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * ----------------------------------------------------------------------
 *
 * This class hold the Tonemapping Options
 *
 * Original Work
 * @author Giuseppe Rota <grota@users.sourceforge.net>
 * @author Franco Comida <fcomida@users.sourceforge.net>
 *
 * New Design based on class rather then struct
 * @author Davide Anastasia <davideanastasia@users.sourceforge.net>
 *
 */

#include <QApplication>
#include <QTextStream>
#include <QFile>
#include <climits>

#include "Common/config.h"
#include "Core/TonemappingOptions.h"
#include "TonemappingOperators/pfstmdefaultparams.h"

void TonemappingOptions::setDefaultTonemapParameters()
{
    // Mantiuk06
    operator_options.mantiuk06options.contrastfactor = MANTIUK06_CONTRAST_FACTOR;
    operator_options.mantiuk06options.saturationfactor = MANTIUK06_SATURATION_FACTOR;
    operator_options.mantiuk06options.detailfactor = MANTIUK06_DETAIL_FACTOR;
    operator_options.mantiuk06options.contrastequalization = MANTIUK06_CONSTRAST_EQUALIZATION;

    // Mantiuk08
    operator_options.mantiuk08options.colorsaturation = MANTIUK08_COLOR_SATURATION;
    operator_options.mantiuk08options.contrastenhancement = MANTIUK08_CONTRAST_ENHANCEMENT;
    operator_options.mantiuk08options.luminancelevel = MANTIUK08_LUMINANCE_LEVEL;
    operator_options.mantiuk08options.setluminance = MANTIUK08_SET_LUMINANCE;

    // Fattal
    operator_options.fattaloptions.alpha = FATTAL02_ALPHA;
    operator_options.fattaloptions.beta = FATTAL02_BETA;
    operator_options.fattaloptions.color = FATTAL02_COLOR;
    operator_options.fattaloptions.noiseredux = FATTAL02_NOISE_REDUX;
    operator_options.fattaloptions.newfattal = FATTAL02_NEWFATTAL;

    // Drago
    operator_options.dragooptions.bias = DRAGO03_BIAS;

    // Durand
    operator_options.durandoptions.spatial = DURAND02_SPATIAL;
    operator_options.durandoptions.range = DURAND02_RANGE;
    operator_options.durandoptions.base = DURAND02_BASE;

    // Reinhard 02
    operator_options.reinhard02options.scales = REINHARD02_SCALES;
    operator_options.reinhard02options.key = REINHARD02_KEY;
    operator_options.reinhard02options.phi = REINHARD02_PHI;
    operator_options.reinhard02options.range = REINHARD02_RANGE;
    operator_options.reinhard02options.lower = REINHARD02_LOWER;
    operator_options.reinhard02options.upper = REINHARD02_UPPER;

    // Reinhard 05
    operator_options.reinhard05options.brightness = REINHARD05_BRIGHTNESS;
    operator_options.reinhard05options.chromaticAdaptation = REINHARD05_CHROMATIC_ADAPTATION;
    operator_options.reinhard05options.lightAdaptation = REINHARD05_LIGHT_ADAPTATION;

    // Ashikhmin
    operator_options.ashikhminoptions.simple = ASHIKHMIN_SIMPLE;
    operator_options.ashikhminoptions.eq2 = ASHIKHMIN_EQ2;
    operator_options.ashikhminoptions.lct = ASHIKHMIN_LCT;

    // Pattanaik
    operator_options.pattanaikoptions.autolum = PATTANAIK00_AUTOLUM;
    operator_options.pattanaikoptions.local = PATTANAIK00_LOCAL;
    operator_options.pattanaikoptions.cone = PATTANAIK00_CONE;
    operator_options.pattanaikoptions.rod = PATTANAIK00_ROD;
    operator_options.pattanaikoptions.multiplier = PATTANAIK00_MULTIPLIER;
}

void TonemappingOptions::setDefaultParameters()
{
    // TM Defaults
    setDefaultTonemapParameters();

    origxsize = INT_MAX;
    xsize = INT_MAX;
    pregamma = 1.0f;
    tonemapSelection = false;
    tonemapOriginal = true;
    tmoperator = mantiuk06;
    tmoperator_str = "Mantiuk '06";

    selection_x_up_left = 0;
    selection_y_up_left = 0;
    selection_x_bottom_right = INT_MAX;
    selection_y_bottom_right = INT_MAX;
}


TonemappingOptions* TMOptionsOperations::parseFile(QString fname) {
        QFile file(fname);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text) || file.size()==0) {
                throw (QApplication::tr("ERROR: cannot load Tone Mapping Setting file: ")+fname);
                return NULL;
        }

        TonemappingOptions *toreturn = new TonemappingOptions;
        memset(toreturn, 0, sizeof *toreturn);

        QTextStream in(&file);
        QString field,value;

        while (!in.atEnd()) {
                QString line = in.readLine();
                //skip comments
                if (line.startsWith('#'))
                        continue;

                field=line.section('=',0,0); //get the field
                value=line.section('=',1,1); //get the value
                if (field=="TMOSETTINGSVERSION") {
                        if (value != TMOSETTINGSVERSION) {
                                delete toreturn;
                                throw (QApplication::tr("ERROR: File too old, cannot parse Tone Mapping Setting file: ")+fname);
// 				return NULL;
                        }
                } else if (field=="XSIZE") {
						toreturn->xsize=value.toInt();
				} else if (field=="QUALITY") {
						toreturn->quality=value.toInt();
				} else if (field=="TMO") {
                        if (value=="Ashikhmin02") {
                                toreturn->tmoperator=ashikhmin;
                        } else if (value == "Drago03") {
                                toreturn->tmoperator=drago;
                        } else if (value == "Durand02") {
                                toreturn->tmoperator=durand;
                        } else if (value == "Fattal02") {
                                toreturn->tmoperator=fattal;
                        } else if (value == "Pattanaik00") {
                                toreturn->tmoperator=pattanaik;
                        } else if (value == "Reinhard02") {
                                toreturn->tmoperator=reinhard02;
                        } else if (value == "Reinhard05") {
                                toreturn->tmoperator=reinhard05;
                        } else if (value == "Mantiuk06") {
                                toreturn->tmoperator=mantiuk06;
                        } else if (value == "Mantiuk08") {
                                toreturn->tmoperator=mantiuk08;
                        }
                } else if (field=="CONTRASTFACTOR") {
                        toreturn->operator_options.mantiuk06options.contrastfactor=value.toFloat();
                } else if (field=="SATURATIONFACTOR") {
                        toreturn->operator_options.mantiuk06options.saturationfactor=value.toFloat();
                } else if (field=="DETAILFACTOR") {
                        toreturn->operator_options.mantiuk06options.detailfactor=value.toFloat();
                } else if (field=="CONTRASTEQUALIZATION") {
                        toreturn->operator_options.mantiuk06options.contrastequalization=(value == "YES");
                } else if (field=="COLORSATURATION") {
                        toreturn->operator_options.mantiuk08options.colorsaturation=value.toFloat();
                } else if (field=="CONTRASTENHANCEMENT") {
                        toreturn->operator_options.mantiuk08options.contrastenhancement=value.toFloat();
                } else if (field=="LUMINANCELEVEL") {
                        toreturn->operator_options.mantiuk08options.luminancelevel=value.toFloat();
                } else if (field=="SETLUMINANCE") {
                        toreturn->operator_options.mantiuk08options.setluminance=(value == "YES");
                } else if (field=="SIMPLE") {
                        toreturn->operator_options.ashikhminoptions.simple= (value == "YES") ? true : false;
                } else if (field=="EQUATION") {
                        toreturn->operator_options.ashikhminoptions.eq2= (value=="2") ? true : false;
                } else if (field=="CONTRAST") {
                        toreturn->operator_options.ashikhminoptions.lct=value.toFloat();
                } else if (field=="BIAS") {
                        toreturn->operator_options.dragooptions.bias=value.toFloat();
                } else if (field=="SPATIAL") {
                        toreturn->operator_options.durandoptions.spatial=value.toFloat();
                } else if (field=="RANGE") {
                        toreturn->operator_options.durandoptions.range=value.toFloat();
                } else if (field=="BASE") {
                        toreturn->operator_options.durandoptions.base=value.toFloat();
                } else if (field=="ALPHA") {
                        toreturn->operator_options.fattaloptions.alpha=value.toFloat();
                } else if (field=="BETA") {
                        toreturn->operator_options.fattaloptions.beta=value.toFloat();
                } else if (field=="COLOR") {
                        toreturn->operator_options.fattaloptions.color=value.toFloat();
                } else if (field=="NOISE") {
                        toreturn->operator_options.fattaloptions.noiseredux=value.toFloat();
                } else if (field=="OLDFATTAL") {
                        toreturn->operator_options.fattaloptions.newfattal= (value == "NO");
                } else if (field=="MULTIPLIER") {
                        toreturn->operator_options.pattanaikoptions.multiplier=value.toFloat();
                } else if (field=="LOCAL") {
                        toreturn->operator_options.pattanaikoptions.local= (value=="YES");
                } else if (field=="AUTOLUMINANCE") {
                        toreturn->operator_options.pattanaikoptions.autolum= (value=="YES");
                } else if (field=="CONE") {
                        toreturn->operator_options.pattanaikoptions.cone=value.toFloat();
                } else if (field=="ROD") {
                        toreturn->operator_options.pattanaikoptions.rod=value.toFloat();
                } else if (field=="KEY") {
                        toreturn->operator_options.reinhard02options.key=value.toFloat();
                } else if (field=="PHI") {
                        toreturn->operator_options.reinhard02options.phi=value.toFloat();
                } else if (field=="SCALES") {
                        toreturn->operator_options.reinhard02options.scales= (value=="YES") ? true : false;
                } else if (field=="RANGE") {
                        toreturn->operator_options.reinhard02options.range=value.toInt();
                } else if (field=="LOWER") {
                        toreturn->operator_options.reinhard02options.lower=value.toInt();
                } else if (field=="UPPER") {
                        toreturn->operator_options.reinhard02options.upper=value.toInt();
                } else if (field=="BRIGHTNESS") {
                        toreturn->operator_options.reinhard05options.brightness=value.toFloat();
                } else if (field=="CHROMATICADAPTATION") {
                        toreturn->operator_options.reinhard05options.chromaticAdaptation=value.toFloat();
                } else if (field=="LIGHTADAPTATION") {
                        toreturn->operator_options.reinhard05options.lightAdaptation=value.toFloat();
                } else if (field=="PREGAMMA") {
                        toreturn->pregamma=value.toFloat();
                } else {
                        delete toreturn;
                        throw (QApplication::tr("ERROR: cannot parse Tone Mapping Setting file: ")+fname);
// 			return NULL;
                }
        }
        return toreturn;
}

TonemappingOptions* TMOptionsOperations::getDefaultTMOptions() {
        TonemappingOptions *toreturn=new TonemappingOptions;
        //TODO when instantiating the tonemapperThread, check this value: if -2 => create thread with originalsize=-2 (to skip resize the step as we did with the batch tone mapping), else (the user wants to resize) create thread with true originalxsize
        toreturn->xsize=-2;
        return toreturn;
}

TMOptionsOperations::TMOptionsOperations(const TonemappingOptions* opts) : opts(opts) {
}

QString TMOptionsOperations::getPostfix() {
        QString postfix=QString("pregamma_%1_").arg(opts->pregamma);
        switch (opts->tmoperator) {
        case mantiuk06: {
                postfix+="mantiuk06_";
                float contrastfactor=opts->operator_options.mantiuk06options.contrastfactor;
                float saturationfactor=opts->operator_options.mantiuk06options.saturationfactor;
                float detailfactor=opts->operator_options.mantiuk06options.detailfactor;
                bool contrast_eq=opts->operator_options.mantiuk06options.contrastequalization;
                if (contrast_eq) {
                        postfix+=QString("contrast_equalization_%1_").arg(contrastfactor);
                } else {
                        postfix+=QString("contrast_mapping_%1_").arg(contrastfactor);
                }
                postfix+=QString("saturation_factor_%1_").arg(saturationfactor);
                postfix+=QString("detail_factor_%1").arg(detailfactor);
                }
                break;
        case mantiuk08: {
                postfix+="mantiuk08_";
                float colorsaturation=opts->operator_options.mantiuk08options.colorsaturation;
                float contrastenhancement=opts->operator_options.mantiuk08options.contrastenhancement;
                float luminancelevel=opts->operator_options.mantiuk08options.luminancelevel;
                bool setluminance=opts->operator_options.mantiuk08options.setluminance;
                if (setluminance) {
                        postfix+=QString("luminancelevel_%1_").arg(luminancelevel);
                } else {
                        postfix+=QString("auto_luminance");
                }
                postfix+=QString("colorsaturation_%1_").arg(colorsaturation);
                postfix+=QString("contrastenhancement_%1").arg(contrastenhancement);
                }
                break;
        case fattal: {
                if (!opts->operator_options.fattaloptions.newfattal)
                        postfix+="v1_";
                postfix+="fattal_";
                float alpha=opts->operator_options.fattaloptions.alpha;
                float beta=opts->operator_options.fattaloptions.beta;
                float saturation2=opts->operator_options.fattaloptions.color;
                float noiseredux=opts->operator_options.fattaloptions.noiseredux;
                postfix+=QString("alpha_%1_").arg(alpha);
                postfix+=QString("beta_%1_").arg(beta);
                postfix+=QString("saturation_%1_").arg(saturation2);
                postfix+=QString("noiseredux_%1").arg(noiseredux);
                }
                break;
        case ashikhmin: {
                postfix+="ashikhmin_";
                if (opts->operator_options.ashikhminoptions.simple) {
                        postfix+="-simple";
                } else {
                        if (opts->operator_options.ashikhminoptions.eq2) {
                                postfix+="-eq2_";
                        } else {
                                postfix+="-eq4_";
                        }
                        postfix+=QString("local_%1").arg(opts->operator_options.ashikhminoptions.lct);
                }
                }
                break;
        case drago: {
                postfix+="drago_";
                postfix+=QString("bias_%1").arg(opts->operator_options.dragooptions.bias);
                }
                break;
        case durand: {
                float spatial=opts->operator_options.durandoptions.spatial;
                float range=opts->operator_options.durandoptions.range;
                float base=opts->operator_options.durandoptions.base;
                postfix+="durand_";
                postfix+=QString("spatial_%1_").arg(spatial);
                postfix+=QString("range_%1_").arg(range);
                postfix+=QString("base_%1").arg(base);
                }
                break;
        case pattanaik: {
                float multiplier=opts->operator_options.pattanaikoptions.multiplier;
                float cone=opts->operator_options.pattanaikoptions.cone;
                float rod=opts->operator_options.pattanaikoptions.rod;
                postfix+="pattanaik00_";
                postfix+=QString("mul_%1_").arg(multiplier);
                if (opts->operator_options.pattanaikoptions.local) {
                        postfix+="local";
                } else if (opts->operator_options.pattanaikoptions.autolum) {
                        postfix+="autolum";
                } else {
                        postfix+=QString("cone_%1_").arg(cone);
                        postfix+=QString("rod_%1_").arg(rod);
                }
                }
                break;
        case reinhard02: {
                float key=opts->operator_options.reinhard02options.key;
                float phi=opts->operator_options.reinhard02options.phi;
                int range=opts->operator_options.reinhard02options.range;
                int lower=opts->operator_options.reinhard02options.lower;
                int upper=opts->operator_options.reinhard02options.upper;
                postfix+="reinhard02_";
                postfix+=QString("key_%1_").arg(key);
                postfix+=QString("phi_%1").arg(phi);
                if (opts->operator_options.reinhard02options.scales) {
                        postfix+=QString("_scales_");
                        postfix+=QString("range_%1_").arg(range);
                        postfix+=QString("lower%1_").arg(lower);
                        postfix+=QString("upper%1").arg(upper);
                }
                }
                break;
        case reinhard05: {
                float brightness=opts->operator_options.reinhard05options.brightness;
                float chromaticAdaptation= opts->operator_options.reinhard05options.chromaticAdaptation;
                float lightAdaptation=opts->operator_options.reinhard05options.lightAdaptation;
                postfix+="reinhard05_";
                postfix+=QString("brightness_%1_").arg(brightness);
                postfix+=QString("chromatic_adaptation_%1_").arg(chromaticAdaptation);
                postfix+=QString("light_adaptation_%1").arg(lightAdaptation);
                }
                break;
        }
        return postfix;
}

QString TMOptionsOperations::getCaption() {
        QString caption=QString("PreGamma=%1 ~ ").arg(opts->pregamma);
        switch (opts->tmoperator) {
        case mantiuk06: {
                caption+="Mantiuk06: ~ ";
                float contrastfactor=opts->operator_options.mantiuk06options.contrastfactor;
                float saturationfactor=opts->operator_options.mantiuk06options.saturationfactor;
                float detailfactor=opts->operator_options.mantiuk06options.detailfactor;
                bool contrast_eq=opts->operator_options.mantiuk06options.contrastequalization;
                if (contrast_eq) {
                        caption+=QString("Contrast Equalization=%1 ~ ").arg(contrastfactor);
                } else {
                        caption+=QString("Contrast=%1 ~ ").arg(contrastfactor);
                }
                caption+=QString("Saturation=%1 ~ ").arg(saturationfactor);
                caption+=QString("Detail=%1").arg(detailfactor);
                }
                break;
        case mantiuk08: {
                caption+="Mantiuk08: ~ ";
                float colorsaturation=opts->operator_options.mantiuk08options.colorsaturation;
                float contrastenhancement=opts->operator_options.mantiuk08options.contrastenhancement;
                float luminancelevel=opts->operator_options.mantiuk08options.luminancelevel;
                bool setluminance=opts->operator_options.mantiuk08options.setluminance;
                if (setluminance) {
                        caption+=QString("Luminance Level=%1 ~ ").arg(luminancelevel);
                } else {
                        caption+=QString("Luminance Level=Auto ~ ");
                }
                caption+=QString("Color Saturation=%1 ~ ").arg(colorsaturation);
                caption+=QString("Contrast Enhancement=%1").arg(contrastenhancement);
                }
                break;
        case fattal: {
                if (!opts->operator_options.fattaloptions.newfattal)
                        caption+="V1_";
                float alpha=opts->operator_options.fattaloptions.alpha;
                float beta=opts->operator_options.fattaloptions.beta;
                float saturation2=opts->operator_options.fattaloptions.color;
                float noiseredux=opts->operator_options.fattaloptions.noiseredux;
                caption+="Fattal: ~ ";
                caption+=QString("Alpha=%1 ~ ").arg(alpha);
                caption+=QString("Beta=%1 ~ ").arg(beta);
                caption+=QString("Saturation=%1 ~ ").arg(saturation2);
                caption+=QString("NoiseRedux=%1").arg(noiseredux);
                }
                break;
        case ashikhmin: {
                caption+="Ashikhmin: ~ ";
                if (opts->operator_options.ashikhminoptions.simple) {
                        caption+="simple";
                } else {
                        if (opts->operator_options.ashikhminoptions.eq2) {
                                caption+="Equation 2 ~ ";
                        } else {
                                caption+="Equation 4 ~ ";
                        }
                        caption+=QString("Local=%1").arg(opts->operator_options.ashikhminoptions.lct);
                }
                }
                break;
        case drago: {
                caption+="Drago: ~ ";
                caption+=QString("Bias=%1").arg(opts->operator_options.dragooptions.bias);
                }
                break;
        case durand: {
                float spatial=opts->operator_options.durandoptions.spatial;
                float range=opts->operator_options.durandoptions.range;
                float base=opts->operator_options.durandoptions.base;
                caption+="Durand: ~ ";
                caption+=QString("Spatial=%1 ~ ").arg(spatial);
                caption+=QString("Range=%1 ~ ").arg(range);
                caption+=QString("Base=%1").arg(base);
                }
                break;
        case pattanaik: {
                float multiplier=opts->operator_options.pattanaikoptions.multiplier;
                float cone=opts->operator_options.pattanaikoptions.cone;
                float rod=opts->operator_options.pattanaikoptions.rod;
                caption+="Pattanaik00: ~ ";
                caption+=QString("Multiplier=%1 ~ ").arg(multiplier);
                if (opts->operator_options.pattanaikoptions.local) {
                        caption+="Local";
                } else if (opts->operator_options.pattanaikoptions.autolum) {
                        caption+="AutoLuminance";
                } else {
                        caption+=QString("Cone=%1 ~ ").arg(cone);
                        caption+=QString("Rod=%1 ~ ").arg(rod);
                }
                }
                break;
        case reinhard02: {
                float key=opts->operator_options.reinhard02options.key;
                float phi=opts->operator_options.reinhard02options.phi;
                int range=opts->operator_options.reinhard02options.range;
                int lower=opts->operator_options.reinhard02options.lower;
                int upper=opts->operator_options.reinhard02options.upper;
                caption+="Reinhard02: ~ ";
                caption+=QString("Key=%1 ~ ").arg(key);
                caption+=QString("Phi=%1").arg(phi);
                if (opts->operator_options.reinhard02options.scales) {
                        caption+=QString(" ~ Scales: ~ ");
                        caption+=QString("Range=%1 ~ ").arg(range);
                        caption+=QString("Lower=%1 ~ ").arg(lower);
                        caption+=QString("Upper=%1").arg(upper);
                }
                }
                break;
        case reinhard05: {
                float brightness=opts->operator_options.reinhard05options.brightness;
                float chromaticAdaptation= opts->operator_options.reinhard05options.chromaticAdaptation;
                float lightAdaptation=opts->operator_options.reinhard05options.lightAdaptation;
                caption+="Reinhard05: ~ ";
                caption+=QString("Brightness=%1 ~ ").arg(brightness);
                caption+=QString("Chromatic Adaptation=%1 ~ ").arg(chromaticAdaptation);
                caption+=QString("Light Adaptation=%1").arg(lightAdaptation);
                }
                break;
        }
        return caption;
}

QString TMOptionsOperations::getExifComment() {
        QString exif_comment="Luminance HDR "LUMINANCEVERSION" tonemapping parameters:\n";
        exif_comment+="Operator: ";
        switch (opts->tmoperator) {
        case mantiuk06: {
                float contrastfactor=opts->operator_options.mantiuk06options.contrastfactor;
                float saturationfactor=opts->operator_options.mantiuk06options.saturationfactor;
                float detailfactor=opts->operator_options.mantiuk06options.detailfactor;
                bool contrast_eq=opts->operator_options.mantiuk06options.contrastequalization;
                exif_comment+="Mantiuk06\nParameters:\n";
                if (contrast_eq) {
                        exif_comment+=QString("Contrast Equalization factor: %1\n").arg(contrastfactor);
                } else {
                        exif_comment+=QString("Contrast Mapping factor: %1\n").arg(contrastfactor);
                }
                exif_comment+=QString("Saturation Factor: %1 \n").arg(saturationfactor);
                exif_comment+=QString("Detail Factor: %1 \n").arg(detailfactor);
                }
                break;
        case mantiuk08: {
                float colorsaturation=opts->operator_options.mantiuk08options.colorsaturation;
                float contrastenhancement=opts->operator_options.mantiuk08options.contrastenhancement;
                float luminancelevel=opts->operator_options.mantiuk08options.luminancelevel;
                bool setluminance=opts->operator_options.mantiuk08options.setluminance;
                exif_comment+="Mantiuk08\nParameters:\n";
                if (setluminance) {
                        exif_comment+=QString("Luminance Level: %1 \n").arg(luminancelevel);
                } else {
                        exif_comment+=QString("Luminance Level: Auto \n");
                }
                exif_comment+=QString("Color Saturation: %1 \n").arg(colorsaturation);
                exif_comment+=QString("Contrast Enhancement: %1 \n").arg(contrastenhancement);
                }
                break;
        case fattal: {
                float alpha=opts->operator_options.fattaloptions.alpha;
                float beta=opts->operator_options.fattaloptions.beta;
                float saturation2=opts->operator_options.fattaloptions.color;
                float noiseredux=opts->operator_options.fattaloptions.noiseredux;
                if (!opts->operator_options.fattaloptions.newfattal) {
                        exif_comment+="V1_";
                }
                exif_comment+="Fattal\nParameters:\n";
                exif_comment+=QString("Alpha: %1\n").arg(alpha);
                exif_comment+=QString("Beta: %1\n").arg(beta);
                exif_comment+=QString("Color Saturation: %1 \n").arg(saturation2);
                exif_comment+=QString("Noise Reduction: %1 \n").arg(noiseredux);
                }
                break;
        case ashikhmin: {
                exif_comment+="Ashikhmin\nParameters:\n";
                if (opts->operator_options.ashikhminoptions.simple) {
                        exif_comment+="Simple\n";
                } else {
                        if (opts->operator_options.ashikhminoptions.eq2) {
                                exif_comment+="Equation 2\n";
                        } else {
                                exif_comment+="Equation 4\n";
                        }
                        exif_comment+=QString("Local Contrast value: %1\n").arg(opts->operator_options.ashikhminoptions.lct);
                }
                }
                break;
        case drago: {
                exif_comment+="Drago\nParameters:\n";
                exif_comment+=QString("Bias: %1\n").arg(opts->operator_options.dragooptions.bias);
                }
                break;
        case durand: {
                float spatial=opts->operator_options.durandoptions.spatial;
                float range=opts->operator_options.durandoptions.range;
                float base=opts->operator_options.durandoptions.base;
                exif_comment+="Durand\nParameters:\n";
                exif_comment+=QString("Spatial Kernel Sigma: %1\n").arg(spatial);
                exif_comment+=QString("Range Kernel Sigma: %1\n").arg(range);
                exif_comment+=QString("Base Contrast: %1\n").arg(base);
                }
                break;
        case pattanaik: {
                float multiplier=opts->operator_options.pattanaikoptions.multiplier;
                float cone=opts->operator_options.pattanaikoptions.cone;
                float rod=opts->operator_options.pattanaikoptions.rod;
                exif_comment+="Pattanaik\nParameters:\n";
                exif_comment+=QString("Multiplier: %1\n").arg(multiplier);
                if (opts->operator_options.pattanaikoptions.local) {
                        exif_comment+="Local Tone Mapping\n";
                } else if (opts->operator_options.pattanaikoptions.autolum) {
                        exif_comment+="Con and Rod based on image luminance\n";
                } else {
                        exif_comment+=QString("Cone Level: %1\n").arg(cone);
                        exif_comment+=QString("Rod Level: %1\n").arg(rod);
                }
                }
                break;
        case reinhard02: {
                float key=opts->operator_options.reinhard02options.key;
                float phi=opts->operator_options.reinhard02options.phi;
                int range=opts->operator_options.reinhard02options.range;
                int lower=opts->operator_options.reinhard02options.lower;
                int upper=opts->operator_options.reinhard02options.upper;
                exif_comment+="Reinhard02\nParameters:\n";
                exif_comment+=QString("Key: %1\n").arg(key);
                exif_comment+=QString("Phi: %1\n").arg(phi);
                if (opts->operator_options.reinhard02options.scales) {
                        exif_comment+=QString("Scales\n");
                        exif_comment+=QString("Range: %1\n").arg(range);
                        exif_comment+=QString("Lower: %1\n").arg(lower);
                        exif_comment+=QString("Upper: %1\n").arg(upper);
                }
                }
                break;
        case reinhard05: {
                float brightness = opts->operator_options.reinhard05options.brightness;
                float chromaticAdaptation = opts->operator_options.reinhard05options.chromaticAdaptation;
                float lightAdaptation = opts->operator_options.reinhard05options.lightAdaptation;
                exif_comment+="Reinhard05\nParameters:\n";
                exif_comment+=QString("Brightness: %1\n").arg(brightness);
                exif_comment+=QString("Chromatic Adaptation: %1\n").arg(chromaticAdaptation);
                exif_comment+=QString("Light Adaptation: %1\n").arg(lightAdaptation);
                }
                break;
        }
        exif_comment+=QString("------\nPreGamma: %1\n").arg(opts->pregamma);
        return exif_comment;
}






