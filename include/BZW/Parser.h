/* bzflag
 * Copyright (c) 1993 - 2008 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef __BZW_PARSER_H__
#define __BZW_PARSER_H__

#include <string>
#include <map>
#include <vector>
#include <iostream>

/* bzflag common headers */
#include "common.h"

namespace BZW
{
  /**
   * BZW file Parsing class. Used primarily by BZW::World.
   */
  class Parser
  {
    public:
      class Field;
      class Object;
      class Parameter;

      /// Constructor
      Parser();
      /// Destructor
      ~Parser();

      /**
       * Using previously provided definitions, parses istream using
       * definitions. Use
       */
      bool parse(std::istream& in);

      /**
       * Handle a new object type to parse. Adds a new block-like object
       * structure.
       */
      void manageObject(std::string name, Object& object);

      /**
       * Retrieve the multimap of objects read via parsing.
       * FIXME: This probably shouldn't be sending back a copy of the
       * multimap, and sending a reference seems like a bad idea also.
       */
      std::multimap<std::string, Object> getObjects();

    private:
      std::map<std::string, Object> managedObjects;
      std::multimap<std::string, Object> readObjects;
  };


  /**
   * BZW file parsing interface for Objects, Parameters.
   */
  class Parser::Field
  {
    public:
      /// Constructor
      Field(){ };
      /// Destructor
      virtual ~Field(){ };

      bool isRepeatable(){ return repeatable; }
    protected:
      bool repeatable;
  };

  /**
   * BZW file parsing class for handling Parameters. Used primarily within
   * BZW::World and Parser.
   */
  class Parser::Parameter : public Parser::Field
  {
    public:
      enum ValueType
      {
        NOTHING, ///No values expected. The mere presence of this parameter is required.
        REAL, ///Real numbers, floats.
        STRING, ///A single word.
        ENDLESS_STRING ///All text until end of line.
      };

      union ValueValue //TODO: this is a terrible name.
      {
        float real_value; ///Real numbers, floats.
        std::string* string_value; ///String values.
      };

      //TODO: should I worry about number of values? are N required? etc.
      /// Constructor
      Parameter(enum ValueType type, int number_of_values, bool repeatable);
      /// Destructor
      ~Parameter();

      void parse(std::istream& in);

      std::vector<union ValueValue>* getValues();

    private:
      enum ValueType type;
      int number_of_values;
      /* TODO: Since we know the number of values, must this be a vector? Consider an
       * array.
       */
      std::vector<union ValueValue> values;
  };

  /**
   * BZW file Parsing Object. Used primarily by BZW::World and within Parser.
   */
  class Parser::Object : public Parser::Field
  {
    public:
      //FIXME need copy constructor
      /// Constructor
      Object(bool repeatable);
      /// Destructor
      ~Object();

      void manage(std::string name, Field& field);
      std::multimap<std::string, Field>* getFields();

    private:
      /** This is the text that comes after the identifier. Used
       * primarily by define/group/matrefs I believe...
       */
      std::string name;
      std::map<std::string, Field> managedFields;
      std::multimap<std::string, Field> readFields;
  };

}

#endif // __BZW_PARSER_H__

// Local Variables: ***
// Mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
