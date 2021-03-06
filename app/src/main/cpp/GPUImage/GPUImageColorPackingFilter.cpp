/**
 * Created by lvHiei on 17-4-7.
 * This is a project of GPUImage implemented with c++, you can
 * use it free. if you find some bug please send me a email.
 * My Email is majun_1523@163.com.
 */

#include "GPUImageColorPackingFilter.h"

// 顶点着色器
extern const char _colorPacking_vertex_shader[]=
//"precision mediump float;\n"
"attribute vec4 position;\n"
"attribute vec4 inputTextureCoordinate;\n"
"\n"
"uniform float texelWidth;\n"
"uniform float texelHeight;\n"
"\n"
"varying vec2 upperLeftInputTextureCoordinate;\n"
"varying vec2 upperRightInputTextureCoordinate;\n"
"varying vec2 lowerLeftInputTextureCoordinate;\n"
"varying vec2 lowerRightInputTextureCoordinate;\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = position;\n"
"\n"
"    upperLeftInputTextureCoordinate = inputTextureCoordinate.xy + vec2(-texelWidth, -texelHeight);\n"
"    upperRightInputTextureCoordinate = inputTextureCoordinate.xy + vec2(texelWidth, -texelHeight);\n"
"    lowerLeftInputTextureCoordinate = inputTextureCoordinate.xy + vec2(-texelWidth, texelHeight);\n"
"    lowerRightInputTextureCoordinate = inputTextureCoordinate.xy + vec2(texelWidth, texelHeight);\n"
"}"
;


#ifdef __GLSL_SUPPORT_HIGHP__

// 片元着色器
extern const char _colorPacking_fragment_shader[]=
"precision lowp float;\n"
"\n"
"uniform sampler2D inputImageTexture;\n"
"\n"
"uniform mediump mat3 convolutionMatrix;\n"
"\n"
"varying highp vec2 outputTextureCoordinate;\n"
"\n"
"varying highp vec2 upperLeftInputTextureCoordinate;\n"
"varying highp vec2 upperRightInputTextureCoordinate;\n"
"varying highp vec2 lowerLeftInputTextureCoordinate;\n"
"varying highp vec2 lowerRightInputTextureCoordinate;\n"
"\n"
"void main()\n"
"{\n"
"    float upperLeftIntensity = texture2D(inputImageTexture, upperLeftInputTextureCoordinate).r;\n"
"    float upperRightIntensity = texture2D(inputImageTexture, upperRightInputTextureCoordinate).r;\n"
"    float lowerLeftIntensity = texture2D(inputImageTexture, lowerLeftInputTextureCoordinate).r;\n"
"    float lowerRightIntensity = texture2D(inputImageTexture, lowerRightInputTextureCoordinate).r;\n"
"\n"
"    gl_FragColor = vec4(upperLeftIntensity, upperRightIntensity, lowerLeftIntensity, lowerRightIntensity);\n"
"}"
;

#else

// 片元着色器
extern const char _colorPacking_fragment_shader[]=
"precision mediump float;\n"
"uniform sampler2D inputImageTexture;\n"
"\n"
"uniform mat3 convolutionMatrix;\n"
"\n"
"varying vec2 outputTextureCoordinate;\n"
"\n"
"varying vec2 upperLeftInputTextureCoordinate;\n"
"varying vec2 upperRightInputTextureCoordinate;\n"
"varying vec2 lowerLeftInputTextureCoordinate;\n"
"varying vec2 lowerRightInputTextureCoordinate;\n"
"\n"
"void main()\n"
"{\n"
"    float upperLeftIntensity = texture2D(inputImageTexture, upperLeftInputTextureCoordinate).r;\n"
"    float upperRightIntensity = texture2D(inputImageTexture, upperRightInputTextureCoordinate).r;\n"
"    float lowerLeftIntensity = texture2D(inputImageTexture, lowerLeftInputTextureCoordinate).r;\n"
"    float lowerRightIntensity = texture2D(inputImageTexture, lowerRightInputTextureCoordinate).r;\n"
"\n"
"    gl_FragColor = vec4(upperLeftIntensity, upperRightIntensity, lowerLeftIntensity, lowerRightIntensity);\n"
"}"
;


#endif


GPUImageColorPackingFilter::GPUImageColorPackingFilter()
    : GPUImageFilter(_colorPacking_vertex_shader, _colorPacking_fragment_shader)
{
    updateTexelWidthHeight();

    m_iTexelWidthUniformLocation = -1;
    m_iTexelHeightUniformLocation = -1;

}

GPUImageColorPackingFilter::~GPUImageColorPackingFilter()
{

}

bool GPUImageColorPackingFilter::createProgramExtra()
{
    m_iTexelWidthUniformLocation = glGetUniformLocation(m_uProgram, "texelWidth");
    m_iTexelHeightUniformLocation = glGetUniformLocation(m_uProgram, "texelHeight");

    return true;
}

bool GPUImageColorPackingFilter::beforeDrawExtra()
{
    glUniform1f(m_iTexelWidthUniformLocation, m_fTexelWidth);
    glUniform1f(m_iTexelHeightUniformLocation, m_fTexelHeight);

    return true;
}

void GPUImageColorPackingFilter::setTexelWidth(float width)
{
    m_fTexelWidth = width;
}

void GPUImageColorPackingFilter::setTexelHeight(float height)
{
    m_fTexelHeight = height;
}


void GPUImageColorPackingFilter::setTextureSize(int width, int height) {
    GPUImageFilter::setTextureSize(width, height);
    updateTexelWidthHeight();
}

void GPUImageColorPackingFilter::setTextureRotation(Rotation rotation) {
    GPUImageFilter::setTextureRotation(rotation);
    updateTexelWidthHeight();
}

void GPUImageColorPackingFilter::updateTexelWidthHeight()
{
    if(0 == m_iTextureWidth){
        m_iTextureWidth = 1280;
    }

    if(0 == m_iTextureHeight){
        m_iTextureHeight = 720;
    }

    m_fTexelWidth = 1.0 / m_iTextureWidth;
    m_fTexelHeight = 1.0 / m_iTextureHeight;
}

