#define DBG_TAG "drv.fs"
#include <rtdbg.h>
#include <dfs.h>
#include <dfs_fs.h>
#include <dfs_romfs.h>
#include <dfs_ramfs.h>

const char text[] = "Hello, RT-Thread!";

const struct romfs_dirent children_dir[] = {
    {ROMFS_DIRENT_DIR, "onchip", RT_NULL, 0},
    {ROMFS_DIRENT_DIR, "tf", RT_NULL, 0},
    {ROMFS_DIRENT_DIR, "wq", RT_NULL, 0},
    {ROMFS_DIRENT_DIR, "ram", RT_NULL, 0},
    // {ROMFS_DIRENT_DIR, "tmp", RT_NULL, 0},
    {ROMFS_DIRENT_FILE, "test.txt", (rt_uint8_t *)text, sizeof(text)},
};

const struct romfs_dirent romfs_root = {
    ROMFS_DIRENT_DIR,
    "/",
    (const rt_uint8_t *)children_dir,
    sizeof(children_dir) / sizeof(children_dir[0]),
};

int initFileSystem(void)
{
    if(dfs_mount(RT_NULL, "/", "rom", 0, &romfs_root) != RT_EOK)
    {
        LOG_W("mount romfs failed");
        return RT_ERROR;
    }
    LOG_I("mount romfs success");

    void *ramfsBuffer = rt_malloc(4*1024);
    if(dfs_mount(RT_NULL, "/ram", "ram", 0, dfs_ramfs_create((rt_uint8_t *)ramfsBuffer, sizeof(ramfsBuffer))) != RT_EOK)
    {
        LOG_W("mount ramfs failed");
        rt_free(ramfsBuffer);
        return RT_ERROR;
    }
    LOG_I("mount ramfs success");

    // if(dfs_mount(RT_NULL, "/tmp", "tmp", 0, NULL) != RT_EOK)
    // {
    //     LOG_W("mount tmpfs failed");
    //     return RT_ERROR;
    // }
    // LOG_I("mount tmpfs success");

    return 0;
}
INIT_ENV_EXPORT(initFileSystem);