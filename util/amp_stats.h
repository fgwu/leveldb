#ifndef STORAGE_LEVELDB_UTIL_AMP_STATS_H_
#define STORAGE_LEVELDB_UTIL_AMP_STATS_H_

namespace leveldb {
class AmpStats {
 public:
  AmpStats();
  ~AmpStats();
  std::string ToString() const;
  enum Type {kMem, kImm, kTbl, kCache, kDisk, kReadCnt};
  void Add(Type, double);
  void AddReadCnt(int);

 private:
  double read_lat_mem_;
  long read_lat_mem_cnt_;
  double read_lat_imm_;
  long read_lat_imm_cnt_;
  double read_lat_tbl_;
  long read_lat_tbl_cnt_;
  double read_lat_cache_;
  long read_lat_cache_cnt_;
  double read_lat_disk_;
  long read_lat_disk_cnt_;

  enum { kNumReadCntLimit = 30 };
  int read_cnt_bucket_[kNumReadCntLimit];

};

} // namespace leveldb

#endif // STORAGE_LEVELDB_UTIL_AMP_STATS_H_
