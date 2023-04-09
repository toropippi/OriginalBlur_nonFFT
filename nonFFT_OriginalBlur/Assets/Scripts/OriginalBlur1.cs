using System;
using UnityEngine;

//[ExecuteInEditMode, ImageEffectAllowedInSceneView]
public class OriginalBlur1 : MonoBehaviour
{
    const int width = 1280;
    const int height = 720;
    const int TPB = 64;//ここをかえたらcomputeshaderもかえること
    const int BufWidth = (width + TPB - 1) / TPB * TPB;
    const int BufHeight = (height + TPB - 1) / TPB * TPB;

    [SerializeField, Range(0, 4000)]
    private int _loopnum = 100;

    private RenderTexture dst_renderTextures;

    [SerializeField] UnityEngine.UI.Slider slider;

    int lasttm,nowtm,lastcnt,fcnt;
    string ss;

    //compute shader周り
    [SerializeField] ComputeShader cs;
    ComputeBuffer Buf = null;
    int kernelXBlur, kernelYBlur;
    int cnt;
    private void Awake()
    {
        Application.targetFrameRate = 60;
        cnt = 0;
        lasttm = Gettime();
        lastcnt = cnt;
        cs.SetInt("WX", BufWidth);

        ss = "XBlur";
        ss += (BufWidth / TPB).ToString();
        kernelXBlur = cs.FindKernel(ss);

        ss = "YBlur";
        ss += (BufHeight / TPB).ToString();
        kernelYBlur = cs.FindKernel(ss);
    }


    private void OnRenderImage(RenderTexture source, RenderTexture dest)
    {
        dst_renderTextures = RenderTexture.GetTemporary(width, height, 0, RenderTextureFormat.ARGB32);
        dst_renderTextures.enableRandomWrite = true;
        dst_renderTextures.wrapMode = TextureWrapMode.Clamp;
        dst_renderTextures.Create();

        if (Buf == null)
            Buf = new ComputeBuffer(BufWidth * BufHeight, 4);

        _loopnum = (int)(400.0f * slider.value);
        cs.SetInt("loopnum", _loopnum);


        cs.SetTexture(kernelXBlur, "Tex_ro", source);
        cs.SetBuffer(kernelXBlur, "Buf", Buf);
        //cs.SetTexture(kernelXBlur, "Tex", dst_renderTextures);
        cs.Dispatch(kernelXBlur, 1, height, 1);


        cs.SetBuffer(kernelYBlur, "Buf_ro", Buf);
        //cs.SetBuffer(kernelYBlur, "Buf", Buf);
        //cs.SetTexture(kernelYBlur, "Tex_ro", dst_renderTextures);
        cs.SetTexture(kernelYBlur, "Tex", dst_renderTextures);
        cs.Dispatch(kernelYBlur, 1, width, 1);


        Graphics.Blit(dst_renderTextures, dest);
        RenderTexture.ReleaseTemporary(dst_renderTextures);


        nowtm = Gettime();
        if (lasttm != nowtm) 
        {
            lasttm = nowtm;
            fcnt = cnt - lastcnt;
            lastcnt = cnt;
        }

        cnt++;
    }

    
    void OnGUI()
    {
        int x;
        // グラフィックデバイス名
        GUILayout.Label($"グラフィックデバイス名:{UnityEngine.SystemInfo.graphicsDeviceName}");
        // グラフィックスAPIタイプ（Direct3D11とか）
        var s0 = UnityEngine.SystemInfo.graphicsDeviceVersion;
        GUILayout.Label($"グラフィックスAPIタイプ:{s0}");
        // ビデオメモリサイズ
        x = UnityEngine.SystemInfo.graphicsMemorySize;
        GUILayout.Label($"ビデオメモリサイズ:{x}");
        // シェーダレベル
        GUILayout.Label($"シェーダレベル:{UnityEngine.SystemInfo.graphicsShaderLevel}");
        // コンピュートシェーダが使えるか
        GUILayout.Label($"コンピュートシェーダが使えるか:{UnityEngine.SystemInfo.supportsComputeShaders}");

        GUILayout.Label($"\t\t\t\t\t\t\tフレーム:{cnt}");
        GUILayout.Label($"\t\t\t\t\t\t\tFPS:{fcnt}");
        GUILayout.Label($"\t\t\t\t\t\t\t_loopnum:{_loopnum}");
        GUILayout.Label($"\t\t\t\t\t\t\tss: {ss}");

    }
    int Gettime()
    {
        return DateTime.Now.Second * 1000
            + DateTime.Now.Minute * 60 * 1000 + DateTime.Now.Hour * 60 * 60 * 1000;
    }

    private void OnDestroy()
    {
        Buf.Release();
    }
}

