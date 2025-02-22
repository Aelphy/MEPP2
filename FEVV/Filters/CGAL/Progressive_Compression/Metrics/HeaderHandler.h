// Copyright (c) 2012-2022 University of Lyon and CNRS (France).
// All rights reserved.
//
// This file is part of MEPP2; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 3 of
// the License, or (at your option) any later version.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#pragma once

#include "FEVV/Filters/CGAL/Progressive_Compression/Parameters.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4146 26812 26451)
#endif

#include "HeaderHandler_draco_nowarning.h"

#if defined _MSC_VER
#pragma warning(pop)
#endif

namespace FEVV {

class HeaderHandler
{
public:
  HeaderHandler(const std::vector< double >& dimension,
                const std::vector< double >& init_coord,
                FEVV::Filters::VKEPT_POSITION vkept,
                FEVV::Filters::PREDICTION_TYPE pred,
                int bit_quantization)
  {
    _dimension = dimension;
    _init_coord = init_coord;
    _vkept = vkept;
    _pred = pred;
    _bit_quantization = bit_quantization;
  };

  HeaderHandler()
  {
    _bit_quantization = 16;
    _vkept = FEVV::Filters::VKEPT_POSITION::MIDPOINT;
    _pred = FEVV::Filters::PREDICTION_TYPE::BUTTERFLY;
  };
  ~HeaderHandler(){};

  void encodeTextHeader(const std::string& filepath)
  {
    std::ofstream file_encode;
    file_encode.open(filepath);
    unsigned int kept_int = static_cast< unsigned int >(_vkept);
    file_encode << (kept_int) << std::endl;
    unsigned int prediction_int = static_cast< unsigned int >(_pred);
    file_encode << prediction_int << std::endl;
    unsigned int quantif = static_cast< unsigned int >(_bit_quantization);
    file_encode << quantif << std::endl;

    for(size_t i = 0; i < 3; i++)
    {
      file_encode << _dimension[i] << std::endl;
    }
    for(size_t i = 0; i < 3; i++)
    {
      file_encode << _init_coord[i] << std::endl;
    }

    file_encode.close();
  }

  void decodeTextHeader(std::ifstream &file_decode)
  {

    // file_decode.open(filepath);
    if(file_decode)
    {
      std::string current_line;
      getline(file_decode, current_line, '\n');
      _vkept =
          static_cast< FEVV::Filters::VKEPT_POSITION >(std::stoi(current_line));
      getline(file_decode, current_line, '\n');
      _pred = static_cast< FEVV::Filters::PREDICTION_TYPE >(
          std::stoi(current_line));
      getline(file_decode, current_line, '\n');
      _bit_quantization = std::stoi(current_line);
      for(size_t i = 0; i < 3; i++)
      {
        getline(file_decode, current_line, '\n');
        _dimension.push_back(std::stod(current_line));
      }
      for(size_t i = 0; i < 3; i++)
      {
        getline(file_decode, current_line, '\n');
        _init_coord.push_back(std::stod(current_line));
      }
      // file_decode.close();
    }
  }

  //Encodes header info in the file located at "filepath". Returns the size of the header in bits
  size_t EncodeBinaryHeader(const std::string& filepath
                            )  
  {
    std::fstream output_file;
    output_file.open(
        filepath, std::fstream::out | std::fstream::binary | std::fstream::app);

    draco::EncoderBuffer buffer;
    int vkept_position = static_cast< int >(_vkept);
    int prediction_type = static_cast< int >(_pred);
    draco::EncodeVarint(vkept_position, &buffer);
    draco::EncodeVarint(prediction_type, &buffer);
    draco::EncodeVarint(_bit_quantization, &buffer);
    buffer.Encode(_dimension[0]);
    buffer.Encode(_dimension[1]);
    buffer.Encode(_dimension[2]);
    buffer.Encode(_init_coord[0]);
    buffer.Encode(_init_coord[1]);
    buffer.Encode(_init_coord[2]);
    
    std::list< char > file_buffer;
    file_buffer.insert(file_buffer.begin(),
                       (buffer.buffer())->begin(),
                       (buffer.buffer())->end());
    size_t size_header = file_buffer.size() * 8;
    for(auto buf_it = file_buffer.begin(); buf_it != file_buffer.end();
        buf_it++)
    {
      // std::cout << *buf_it << std::endl;
      output_file << *buf_it;
    }
    output_file.close();
    return size_header;
  }

  bool getUseTexture() const
  { return false;
  }
  //Decodes a header from a draco buffer
  void DecodeBinaryHeader(draco::DecoderBuffer &buffer)
  {
    int vk;
    draco::DecodeVarint(&vk, &buffer);
    _vkept = static_cast< FEVV::Filters::VKEPT_POSITION >(vk);
    int pred;
    draco::DecodeVarint(&pred, &buffer);
    _pred = static_cast< FEVV::Filters::PREDICTION_TYPE >(pred);
    draco::DecodeVarint(&_bit_quantization, &buffer);
    for(size_t i = 0; i < 3; i++)
    {
      double curr;
      buffer.Decode(&curr);
      _dimension.push_back(curr);
    }
    for(size_t i = 0; i < 3; i++)
    {
      double curr;
      buffer.Decode(&curr);
      _init_coord.push_back(curr);
    }
  }
 

  FEVV::Filters::VKEPT_POSITION getVkept() const { return _vkept; }
  FEVV::Filters::PREDICTION_TYPE getPred() const { return _pred; }
  int getQuantization() const { return _bit_quantization; }
  const std::vector< double >& getDimension() const { return _dimension; }
  const std::vector< double >& getInitCoord() const { return _init_coord; }

private:
  std::vector< double > _dimension;
  std::vector< double > _init_coord;
  FEVV::Filters::VKEPT_POSITION _vkept;
  FEVV::Filters::PREDICTION_TYPE _pred;
  int _bit_quantization;
};

} // namespace FEVV
