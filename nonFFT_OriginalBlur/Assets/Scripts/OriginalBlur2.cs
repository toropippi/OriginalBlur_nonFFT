using UnityEngine;

//[ExecuteInEditMode, ImageEffectAllowedInSceneView]
public class OriginalBlur2 : MonoBehaviour
{
    const int width = 1024;
    const int height = 1024;

    [SerializeField, Range(0, 30)]
    private int _loopnum = 3;

    [SerializeField, Range(0, 12)]
    private int _loopnum_octave = 4;

    private RenderTexture dst_renderTextures;

    //compute shader周り
    [SerializeField] ComputeShader cs;
    ComputeBuffer Buf = null;
    int kernelXBlur, kernelYBlur;
    int cnt;

    private void Awake()
    {
        cnt = 0;
    }


    private void OnRenderImage(RenderTexture source, RenderTexture dest)
    {
        dst_renderTextures = RenderTexture.GetTemporary(width, height, 0, RenderTextureFormat.ARGB32);
        dst_renderTextures.enableRandomWrite = true;
        dst_renderTextures.Create();

        if (Buf == null)
            Buf = new ComputeBuffer(width * height, 4);

        Graphics.Blit(source, dst_renderTextures);


        kernelXBlur = cs.FindKernel("XBlur");
        kernelYBlur = cs.FindKernel("YBlur");
        //cs.SetTexture(kernelXBlur, "Tex_ro", source);
        //cs.SetBuffer(kernelXBlur, "Buf", Buf);
        //cs.SetBuffer(kernelYBlur, "Buf_ro", Buf);
        //cs.SetTexture(kernelYBlur, "Tex", dst_renderTextures);
        cs.SetTexture(kernelXBlur, "Tex", dst_renderTextures);
        cs.SetTexture(kernelYBlur, "Tex", dst_renderTextures);

        cs.SetInt("loopnum", _loopnum);
        cs.SetInt("loopnum_octave", _loopnum_octave);
        
        cs.Dispatch(kernelXBlur, 1, height, 1);
        cs.Dispatch(kernelYBlur, 1, width, 1);
        Graphics.Blit(dst_renderTextures, dest);
        RenderTexture.ReleaseTemporary(dst_renderTextures);
        cnt++;
    }

    
    void OnGUI()
    {
        int x;
        GUILayout.Label($"フレーム:{cnt}");
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
    }
    


    private void OnDestroy()
    {
        Buf.Release();
    }
}

