/**
 * Created by lvHiei on 17-4-7.
 * This is a project of GPUImage implemented with c++, you can
 * use it free. if you find some bug please send me a email.
 * My Email is majun_1523@163.com.
 */

#include "GPUImageColorBurnBlendFilter.h"
#include "../util/FileUtil.h"


#ifdef __GLSL_SUPPORT_HIGHP__

// 片元着色器
extern const char _colorBurnBlend_fragment_shader[]=
"varying highp vec2 textureCoordinate;\n"
"varying highp vec2 textureCoordinate2;\n"
"\n"
"uniform sampler2D inputImageTexture;\n"
"uniform sampler2D inputImageTexture2;\n"
"\n"
"void main()\n"
"{\n"
"    mediump vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);\n"
"    mediump vec4 textureColor2 = texture2D(inputImageTexture2, textureCoordinate2);\n"
"    mediump vec4 whiteColor = vec4(1.0);\n"
"    gl_FragColor = whiteColor - (whiteColor - textureColor) / textureColor2;\n"
"}"
;

#else

// 片元着色器
extern const char _colorBurnBlend_fragment_shader[]=
"precision mediump float;\n"
"varying vec2 textureCoordinate;\n"
"varying vec2 textureCoordinate2;\n"
"\n"
"uniform sampler2D inputImageTexture;\n"
"uniform sampler2D inputImageTexture2;\n"
"\n"
"void main()\n"
"{\n"
"    vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);\n"
"    vec4 textureColor2 = texture2D(inputImageTexture2, textureCoordinate2);\n"
"    vec4 whiteColor = vec4(1.0);\n"
"    gl_FragColor = whiteColor - (whiteColor - textureColor) / textureColor2;\n"
"}"
;

#endif





GPUImageColorBurnBlendFilter::GPUImageColorBurnBlendFilter()
        : GPUImageImageFilter(_colorBurnBlend_fragment_shader)
{

}

GPUImageColorBurnBlendFilter::~GPUImageColorBurnBlendFilter()
{

}

bool GPUImageColorBurnBlendFilter::loadImage()
{
    // this is decode the image of blend.png.
    // in the example we just use the rgba data stored in
    // the file blend.rgba.

    const char* filename = "blend.rgba";

    m_uPicWidth = 720;
    m_uPicHeight = 1280;
    uint32_t fileLen = FileUtil::getFileSize(filename);
    if(fileLen > 0){
        m_pPicDataRGBA = (uint8_t *) malloc(fileLen * sizeof(uint8_t));
        if(!m_pPicDataRGBA){
            return false;
        }

        FileUtil::loadFile(filename, m_pPicDataRGBA, fileLen);
    }
    return true;
}