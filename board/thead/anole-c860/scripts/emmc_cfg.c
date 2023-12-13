#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*----------------------------------------------*
 * The common data type, will be used in the whole project.*
 *----------------------------------------------*/

typedef unsigned char           DH_U8;
typedef unsigned short          DH_U16;
typedef unsigned int            DH_U32;

typedef signed char             DH_S8;
typedef short                   DH_S16;
typedef int                     DH_S32;

#ifndef _M_IX86
    typedef unsigned long long  DH_U64;
    typedef long long           DH_S64;
#else
    typedef __int64             DH_U64;
    typedef __int64             DH_S64;
#endif

typedef char                    DH_CHAR;
typedef void                    DH_VOID;


typedef float                   DH_F32;
typedef double                  DH_F64;

/*----------------------------------------------*
 * const defination                             *
 *----------------------------------------------*/
typedef enum {
    DH_FALSE = 0,
    DH_TRUE  = 1,
} DH_BOOL;

#ifndef NULL
    #define NULL    0L
#endif

#define DH_NULL     0L
#define DH_SUCCESS  0
#define DH_FAILURE  (-1)
#define DH_ETIMEOUT  (-2)        /* �ȴ���ʱ*/
#define DH_EINTR     (-3)        /* ���ź��ж�*/


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#define CFG_CRC_INIT_VAL    (0x55aa55aa)


#define IMG_MAX_HEADER_SIZE             (128)

/* SPI NOR/NAND Command */
#define SPI_FLASH_CMD_READ              (0x03)
#define SPI_FLASH_CMD_READ_STATUS       (0x05)
#define SPI_FLASH_CMD_WRITE_ENABLE      (0x06)
#define SPI_FLASH_CMD_READ_FEATURE      (0x0F)
#define SPI_FLASH_CMD_READ_TOCACHE      (0x13)
#define SPI_FLASH_CMD_WRITE_FEATURE     (0x1F)
#define SPI_FLASH_CMD_WRITE_SR1         (0x31)
#define SPI_FLASH_CMD_READ_SR1          (0x35)
#define SPI_FLASH_CMD_FAST_READ_QUAD    (0x6B)
#define SPI_FLASH_CMD_READID            (0x9F)
#define SPI_FLASH_CMD_QUAD_IO_READ      (0xEB)


/* SPI nand flash feature settings */
#define SPI_FS_PROTECTION           (0xA0)
#define SPI_FS_FEATURE0             (0xB0)
#define SPI_FS_STATUS0              (0xC0)
#define SPI_FS_FEATURE1             (0xD0)
#define SPI_FS_STATUS1              (0xF0)

#define SPI_QE_MASK		            (0x01)
#define SPI_QE_ENABLE		        (0x01)



/* SPI ���*/

#define SPIO_IO_DOMAIN_MASK     (0x0F)

/* ========�������========*/
#define SPI_IO_MODE_MASK        (0x0F)
#define SPI_IO_MODE_SHIFT       (0x00)
/* ������*/
#define SPI_IO_MODE_1C1A1D      (0x00)

/* ˫����*/
#define SPI_IO_MODE_1C1A2D      (0x01)
#define SPI_IO_MODE_1C2A2D      (0x02)

/* ������*/
#define SPI_IO_MODE_1C1A4D      (0x03)
#define SPI_IO_MODE_1C4A4D      (0x04)

/* DUMMY */
#define SPI_IO_DUMMY_MASK       (0xF0)
#define SPI_IO_DUMMY_SHIFT      (0x04)
#define SPI_IO_DUMMY_NULL	    (0x00)
#define SPI_IO_DUMMY_CYCLE2     (0x02)
#define SPI_IO_DUMMY_CYCLE4     (0x04)
#define SPI_IO_DUMMY_CYCLE6     (0x06)
#define SPI_IO_DUMMY_CYCLE8     (0x08)


#define SPI_IO_SET_ATTR(iomode, dummycycle)   (((iomode & SPIO_IO_DOMAIN_MASK) << SPI_IO_MODE_SHIFT) \
                                         |((dummycycle & SPIO_IO_DOMAIN_MASK) << SPI_IO_DUMMY_SHIFT))



/* Flash����ManufactureID*/
#define FLASH_MANUFID_GD        (0xC8)
#define FLASH_MANUFID_MX        (0xC2)


typedef struct tagImg
{
    /* image info */
    DH_U32 u32StorageType;
    DH_U32 u32StorageAddr;
    DH_U32 u32DownloadAddr;
    DH_U32 u32FileSize;
    DH_U32 u32PageSize;
    DH_U32 u32ColDummy;
    DH_U32 u32FifoDep;
}DH_IMAGE_S;


/* disk���� */
#define DH_COMM_SPI_NORFLASH            (0)
#define DH_COMM_SPI_NANDFLASH           (1)
#define DH_COMM_PRARLLEL_NORFLASH       (2)
#define DH_COMM_PARALLEL_NANDFLASH      (3)
#define DH_COMM_EMMC                    (4)
#define DH_COMM_PCIE                    (5)

/* ECC���� */
#define DH_COMM_ECC_DISABLE         (0)
#define DH_COMM_ECC_ENABLE          (1)
#define DH_COMM_ECC_SOC             (2)
#define DH_COMM_ECC_CRC32           (3)

/* Image Type */
#define DH_IMGTYPE_BIN      (0)
#define DH_IMGTYPE_UIMG     (1)
#define DH_IMGTYPE_SIGN     (2)

/* �洢���豸���� */
typedef struct tagCommDiskInfo
{
     /* Flash���� */
    DH_U8 u8FlashType;
    
    /* ���ݸ��� */
    DH_U8 u8BakCnt;
    
    /* ���β��Һÿ�������������󻵿���
        0 �� Ĭ��Ϊ16
     */
    DH_U8 u8MaxBadBlk;
    
    /* ECC���� */
    DH_U8 u8EccCfg;
    
    
    /* ECC MARK */
    DH_U8 u8EccMask;
    
    /* ECC �����޷��޸�ֵ */
    DH_U8 u8EccNotCorrected;
    

    /* Crc32У�鴰�ڳ��ȣ����Ϊ0�� �����������CrcУ��
    ���鰴��128��256�ĳ��Ƚ������á�������ó�128��ʵ������������£�
    124Bytes(Data) + 4Bytes(Crc32) = 128Bytes
    */
    DH_U16 u16CrcSectSize;

    /* Plane Info */
    DH_U8 u8NeedPlaneSel;
    DH_U8 u8PlaneBit;
    
    /* PageSize, ��λΪByte */
    DH_U16 u16PageSize;
    
    /* Block/Sector ��С����λΪByte */
    DH_U32 u32BlkSize;
    
    /* PageAddr Shift, Ϊ��ַ��Ҫ���Ƶĸ��� */
    DH_U8 u8DummySht;
    
    /* ҳ��ַλ�� */
    DH_U8 u8PageBW;
    
    /*
     ��PageSize���ҵ�һ��1Bit�õ�
     ��� PageSize = 0x800
     �� u8PageAddrShift = 11;
    */
    DH_U8 u8PageAddrShift;
    
    /* Parallel Read Cycle
    0 : 5-Cycle
    1 : 4-Cycle
    */
    DH_U8 u8Cycle4;
    
    /* spare �ռ� */
    DH_U16 u16SpareSize;
    
    /* ECC_CAP, parallel */
    DH_U8 u8EccCap;

    /* ������Ҫ����ҳ������ESMT��Ҫ2�� */
    DH_U8 u8BadChkPages;
    
    /* ���б��� */
    DH_U8 u8Rev[4];
}DH_COMM_DISKINFO_S;

/* �󻪱�־ͷ */
typedef struct tagCommDhFlagInfo
{
    /* �󻪾����־ */
    DH_U32 u32Flag;
    
    /* ����Դ��ַ */
    DH_U32 u32ROMAddr;
    
    /* Boot�м�ʹ�õ��ڲ�SRAM��ַ */
    DH_U32 u32RAMAddr;
    
    /* Boot�׶μ��صĳ��� */
    DH_U32 u32BootLength;
    
    /* ���ص�Ŀ���ַ */
    DH_U32 u32UBootDDRAddr;
    
    /* ���س��� */
    DH_U32 u32UBootLength;
    
    /* ������Ϣ */
    DH_COMM_DISKINFO_S stDiskInfo;
    
    /* �����ã��������Hex�ļ���CrcУ�� */
    DH_U32 u32HexCrc;
    
    /* ����λ */
    DH_U32 u32UBootFlashAddr;
    
    /* У��λ */
    DH_U32 u32Crc;
}DH_COMM_DHFLAG_S;


/* �󻪱�־ͷv3 */
typedef struct tagCommDhFlagInfoV3
{
    /* �󻪾����־ */
    DH_U32 u32Flag;

    DH_U32 u32CfgFlashAddr;
    DH_U32 u32CfgCnt;

    /* miniboot flash and ram addr, image size, image offset, bakup count*/
    DH_U32 u32MBFlashAddr;
    DH_U32 u32MBRamAddr;
    DH_U32 u32MBSize;
    DH_U32 u32MBOffset;
    DH_U32 u32MBBakupCnt;
    DH_U32 u32MBImgType;
    
    /* uboot info */
    DH_U32 u32UBFlashAddr;
    DH_U32 u32UBRamAddr;
    DH_U32 u32UBSize;
    DH_U32 u32UBOffset;
    DH_U32 u32UBBakupCnt;
    DH_U32 u32UBImgType;

    /* ������Ϣ */
    DH_COMM_DISKINFO_S stDiskInfo;

    DH_U32 rev[7];

    /* Silent */
    DH_U32 u32Printf;

    /* У��λ */
    DH_U32 u32ImgCrc;
    DH_U32 u32FlagCrc;
}DH_COMM_DHFLAG_V3_S;




#define IH_MAGIC	0x27051956	/* Image Magic Number		*/
#define IH_NMLEN		32	/* Image Name Length		*/
typedef struct image_header {
	DH_U32		ih_magic;	/* Image Header Magic Number	*/
	DH_U32		ih_hcrc;	/* Image Header CRC Checksum	*/
	DH_U32		ih_time;	/* Image Creation Timestamp	*/
	DH_U32		ih_size;	/* Image Data Size		*/
	DH_U32		ih_load;	/* Data	 Load  Address		*/
	DH_U32		ih_ep;		/* Entry Point Address		*/
	DH_U32		ih_dcrc;	/* Image Data CRC Checksum	*/
	DH_U8		ih_os;		/* Operating System		*/
	DH_U8		ih_arch;	/* CPU architecture		*/
	DH_U8		ih_type;	/* Image Type			*/
	DH_U8		ih_comp;	/* Compression Type		*/
	DH_U8		ih_name[IH_NMLEN];	/* Image Name		*/
} image_header_t;

struct dhc_security_boot
{
    unsigned int rsa_len;
    unsigned int rsa_n0inv;
    unsigned int rsa_modules[64];
    unsigned int rsa_rr[64];

    unsigned char sign[256];
};

#define RSA_KEY_LEN     (520)



/* �󻪱�־ͷ���� */
#define DH_COMM_FLAG_SIZE       (sizeof(DH_COMM_DHFLAG_S))


/* FlashID 
SNOR : SPI NOR
SNAND : SPI NAND
PNAND : Parallel NAND
*/
#define FLASHID_SNOR_GD25Q127       (0x001840C8)
#define FLASHID_SNAND_W25N01G       (0x0021AAEF)


#define DAHUA_FLAG_V3               (0x33646874)
#define DAHUA_FLAG		            (0x0C646874)
#define DAHUA_BLOCK_HDRFLAG         (0x62626864)

#define DH_DEFAULT_MAX_BADCANT      (16)
#define DH_DEFAULT_MAX_TRY          (4)

/* argc */
#define DH_ARGC_EXEC        (0)
#define DH_ARGC_VERSION     (1)
#define DH_ARGC_IMGTYPE     (2)
#define DH_ARGC_CONFIG      (3)
#define DH_ARGC_OUTPUT      (4)
#define DH_ARGC_INPUT_MB    (5)
#define DH_ARGC_INPUT_UB    (6)
#define DH_ARGC_FLASH_BASE  (7)
#define DH_ARGC_FLASH_TYPE  (8)
#define DH_ARGC_KEY_DIR     (9)
#define DH_ARGC_KEY_NAME    (10)
#define DH_ARGC_KEY_GENFILE (11)


DH_COMM_DHFLAG_V3_S gastdh8000_emmc_cfg =
{
    .u32Flag = DAHUA_FLAG_V3,
    
    .u32CfgFlashAddr = 0,
    .u32CfgCnt = 0,
    
    .u32MBFlashAddr = 0,
    .u32MBRamAddr = 0xfe410000,
    .u32MBSize = 0,
    .u32MBOffset = 0x00,
    .u32MBBakupCnt = 0x00,
    
    .u32UBFlashAddr = 0x40000,
    .u32UBRamAddr = 0x17800000,
    .u32UBSize = 0,
    .u32UBOffset = 0,
    .u32UBBakupCnt = 0,
    
    .stDiskInfo = 
    {
        .u8FlashType = DH_COMM_EMMC,
        .u8MaxBadBlk = 0,
        .u16CrcSectSize = 0,
        .u16PageSize = 512,
        .u32BlkSize = 512,
    },
    
    .u32Printf = 1,
};
