using UnityEngine;

//[ExecuteInEditMode, ImageEffectAllowedInSceneView]
public class OriginalBlur2 : MonoBehaviour
{
    const int width = 1920;
    const int height = 1080;

    [SerializeField, Range(0, 30)]
    private int _loopnum = 3;

    [SerializeField, Range(0, 12)]
    private int _loopnum_octave = 4;

    private RenderTexture dst_renderTextures;

    //compute shader����
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

        kernelXBlur = cs.FindKernel("XBlur");
        kernelYBlur = cs.FindKernel("YBlur");
        cs.SetTexture(kernelXBlur, "Tex_ro", source);
        cs.SetBuffer(kernelXBlur, "Buf", Buf);
        cs.SetBuffer(kernelYBlur, "Buf_ro", Buf);
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
        GUILayout.Label($"�t���[��:{cnt}");
        // �O���t�B�b�N�f�o�C�X��
        GUILayout.Label($"�O���t�B�b�N�f�o�C�X��:{UnityEngine.SystemInfo.graphicsDeviceName}");
        // �O���t�B�b�N�XAPI�^�C�v�iDirect3D11�Ƃ��j
        var s0 = UnityEngine.SystemInfo.graphicsDeviceVersion;
        GUILayout.Label($"�O���t�B�b�N�XAPI�^�C�v:{s0}");
        // �r�f�I�������T�C�Y
        x = UnityEngine.SystemInfo.graphicsMemorySize;
        GUILayout.Label($"�r�f�I�������T�C�Y:{x}");
        // �V�F�[�_���x��
        GUILayout.Label($"�V�F�[�_���x��:{UnityEngine.SystemInfo.graphicsShaderLevel}");
        // �R���s���[�g�V�F�[�_���g���邩
        GUILayout.Label($"�R���s���[�g�V�F�[�_���g���邩:{UnityEngine.SystemInfo.supportsComputeShaders}");
    }
    


    private void OnDestroy()
    {
        Buf.Release();
    }
}

