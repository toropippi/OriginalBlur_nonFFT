#include <boost/preprocessor.hpp>

//celr only code
#define OFFSETADD 2



#define macro2(n,data) float BOOST_PP_CAT(celr, n);
#define INITCEL(z,n,data) macro2(n,data)

#define SHARETOREG1(z,n,xpt) \
i = BOOST_PP_CAT(block, BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2))[id.x * BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2) + n];\
BOOST_PP_CAT(celr, n) = (i % 256);\

#define REGTOSHARE1(z,n,xpt) \
i = BOOST_PP_CAT(block, BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2))[id.x * BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2) + n];\
i = ((uint)BOOST_PP_CAT(celr, n)) + (i&0xffffff00);\
BOOST_PP_CAT(block, BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2))[id.x * BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2) + n] = i;\
BOOST_PP_CAT(celr, n)=(i / 256 % 256);\

#define REGTOSHARE2(z,n,xpt) \
i = BOOST_PP_CAT(block, BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2))[id.x * BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2) + n];\
i = ((uint)BOOST_PP_CAT(celr, n))*256 + (i&0xffff00ff);\
BOOST_PP_CAT(block, BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2))[id.x * BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2) + n] = i;\
BOOST_PP_CAT(celr, n)=(i / 65536 % 256);\

#define REGTOSHARE3(z,n,xpt) \
i = BOOST_PP_CAT(block, BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2))[id.x * BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2) + n];\
i = ((uint)BOOST_PP_CAT(celr, n))*65536 + (i&0xff00ffff);\
BOOST_PP_CAT(block, BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2))[id.x * BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2) + n] = i;\

#define MAINLOOP(z,n,xpt) \
regr[n & 1] = BOOST_PP_CAT(celr, BOOST_PP_ADD(n, 1)) * WLR;\
regr[(n & 1) ^ 1] = regr[(n & 1) ^ 1] + BOOST_PP_CAT(celr, BOOST_PP_ADD(n, 2)) * WLR;\
BOOST_PP_CAT(celr, BOOST_PP_ADD(n, 1)) = BOOST_PP_CAT(celr, BOOST_PP_ADD(n, 1)) * W + regr[(n & 1) ^ 1];\

#define TTOS(z_,n,xpt) \
f4 = Tex_ro[float2(n * TPB + id.x, id.y)]; \
BOOST_PP_CAT(block, BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2))[n * TPB + id.x] = ((uint)(f4.x * FTOI)) + ((uint)(f4.y * FTOI)) * 256 + ((uint)(f4.z * FTOI)) * 65536; \

#define STOG(z,n,xpt) \
i = BOOST_PP_CAT(block, BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2))[n * TPB + id.x]; \
Buf[(n * TPB + id.x) + id.y * TPB * NPT] = i; \

#define STOT(z_,n,xpt) \
f4.w = 1; \
i = BOOST_PP_CAT(block, BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2))[n * TPB + id.x]; \
f4.x = (i % 256); \
f4.y = (i / 256 % 256); \
f4.z = (i / 65536 % 256); \
f4.xyz /= FTOI; \
Tex[float2(id.x + n * TPB ,id.y)] = f4; \

#define STOX(z_,n,xpt) \
i = BOOST_PP_CAT(block, BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2))[n * TPB + id.x]; \
STORE(n,i,BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2));\





//
//
//
//BOOST_PP_MUL(BOOST_PP_ADD(xpt,6),2)
//
//#define STORE(n,i,NPT) Buf[(n * TPB + id.x) + id.y * NPT * TPB] = i;
//#define STORE(n,i,NPT) f4.w = 1;f4.x = (i % 256);f4.y = (i / 256 % 256);f4.z = (i / 65536 % 256);f4.xyz /= FTOI;Tex[float2(id.x + n * TPB, id.y)] = f4;



#define ALLCODE(z,xpt,data_) \
 \
groupshared uint BOOST_PP_CAT(block, BOOST_PP_MUL(BOOST_PP_ADD(xpt,OFFSETADD),2))[BOOST_PP_MUL(BOOST_PP_ADD(xpt,OFFSETADD),2) * TPB]; \
[numthreads(TPB, 1, 1)] \
void BOOST_PP_CAT(XBlur, BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2))(uint2 id : SV_DispatchThreadID) \
{\
    float4 f4;uint i, j; float regr[2];\
    BOOST_PP_REPEAT(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2), INITCEL, 0);\
    BOOST_PP_REPEAT(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2), TTOS, xpt);\
    GroupMemoryBarrierWithGroupSync();\
    BOOST_PP_REPEAT(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2), SHARETOREG1, xpt);\
\
\
\
    for (j = 0; j < (uint)loopnum; j++)\
    {\
        GroupMemoryBarrierWithGroupSync();\
        fblock0r[id.x] = celr0; fblock1r[id.x] = BOOST_PP_CAT(celr, BOOST_PP_SUB(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2), 1));\
        GroupMemoryBarrierWithGroupSync();\
        regr[1] = celr0 * WLR;regr[0] = celr1 * WLR + fblock1r[(id.x + TPB - 1) % TPB] * WLR;celr0 = celr0 * W + regr[0];\
        BOOST_PP_REPEAT(BOOST_PP_SUB(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2), 2), MAINLOOP, xpt);\
        regr[(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2) & 1) ^ 1] = regr[(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2) & 1) ^ 1] + fblock0r[(id.x + 1) % TPB] * WLR;\
        BOOST_PP_CAT(celr, BOOST_PP_SUB(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2), 1)) = BOOST_PP_CAT(celr, BOOST_PP_SUB(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2), 1)) * W + regr[(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2) & 1) ^ 1];\
    }\
    GroupMemoryBarrierWithGroupSync();\
    BOOST_PP_REPEAT(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2), REGTOSHARE1, xpt);\
\
    for (j = 0; j < (uint)loopnum; j++)\
    {\
        GroupMemoryBarrierWithGroupSync();\
        fblock0r[id.x] = celr0; fblock1r[id.x] = BOOST_PP_CAT(celr, BOOST_PP_SUB(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2), 1));\
        GroupMemoryBarrierWithGroupSync();\
        regr[1] = celr0 * WLR;regr[0] = celr1 * WLR + fblock1r[(id.x + TPB - 1) % TPB] * WLR;celr0 = celr0 * W + regr[0];\
        BOOST_PP_REPEAT(BOOST_PP_SUB(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2), 2), MAINLOOP, xpt);\
        regr[(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2) & 1) ^ 1] = regr[(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2) & 1) ^ 1] + fblock0r[(id.x + 1) % TPB] * WLR;\
        BOOST_PP_CAT(celr, BOOST_PP_SUB(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2), 1)) = BOOST_PP_CAT(celr, BOOST_PP_SUB(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2), 1)) * W + regr[(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2) & 1) ^ 1];\
    }\
    GroupMemoryBarrierWithGroupSync();\
    BOOST_PP_REPEAT(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2), REGTOSHARE2, xpt);\
\
    for (j = 0; j < (uint)loopnum; j++)\
    {\
        GroupMemoryBarrierWithGroupSync();\
        fblock0r[id.x] = celr0; fblock1r[id.x] = BOOST_PP_CAT(celr, BOOST_PP_SUB(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2), 1));\
        GroupMemoryBarrierWithGroupSync();\
        regr[1] = celr0 * WLR;regr[0] = celr1 * WLR + fblock1r[(id.x + TPB - 1) % TPB] * WLR;celr0 = celr0 * W + regr[0];\
        BOOST_PP_REPEAT(BOOST_PP_SUB(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2), 2), MAINLOOP, xpt);\
        regr[(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2) & 1) ^ 1] = regr[(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2) & 1) ^ 1] + fblock0r[(id.x + 1) % TPB] * WLR;\
        BOOST_PP_CAT(celr, BOOST_PP_SUB(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2), 1)) = BOOST_PP_CAT(celr, BOOST_PP_SUB(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2), 1)) * W + regr[(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2) & 1) ^ 1];\
    }\
    GroupMemoryBarrierWithGroupSync();\
    BOOST_PP_REPEAT(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2), REGTOSHARE3, xpt);\
\
\
\
\
    GroupMemoryBarrierWithGroupSync();\
    BOOST_PP_REPEAT(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2), STOX, xpt);\
}\
\







BOOST_PP_REPEAT(10, ALLCODE, _)

