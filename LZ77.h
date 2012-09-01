#pragma once
u32 UncompressLZ(const  u8 *srcp, u8 *destp );
u32 CompressLZ(const u8 *srcp, u32 size, u8 *dstp/*, void *work*/);