//====== Copyright (c) 2012, Valve Corporation, All rights reserved. ========//
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
// Redistributions in binary form must reproduce the above copyright notice, 
// this list of conditions and the following disclaimer in the documentation 
// and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
// THE POSSIBILITY OF SUCH DAMAGE.
//===========================================================================//

#include <stdarg.h>
#include <conio.h>
#include "demofile.h"
#include "demofiledump.h"

#include "google/protobuf/descriptor.h"
#include "google/protobuf/reflection_ops.h"
#include "google/protobuf/descriptor.pb.h"

#include "generated_proto/netmessages.pb.h"
#include "generated_proto/cstrike15_usermessages.pb.h"

__declspec( noreturn ) void fatal_errorf( const char* fmt, ... )
{
    va_list  vlist;
    char buf[ 1024 ];

    va_start( vlist, fmt);
    vsnprintf( buf, sizeof( buf ), fmt, vlist );
    buf[ sizeof( buf ) - 1 ] = 0;
    va_end( vlist );

    fprintf( stderr, "\nERROR: %s\n", buf );
    exit( -1 );
}

bool CDemoFileDump::Open( const char *filename )
{
    if ( !m_demofile.Open( filename ) )
    {
        fprintf( stderr, "Couldn't open '%s'\n", filename );
        return false;
    }

    return true;
}

void CDemoFileDump::MsgPrintf( const ::google::protobuf::Message& msg, int size, const char *fmt, ... )
{
    va_list vlist;
    const std::string& TypeName = msg.GetTypeName();

    // Print the message type and size
    printf( "---- %s (%d bytes) -----------------\n", TypeName.c_str(), size );

    va_start( vlist, fmt);
    vprintf( fmt, vlist );
    va_end( vlist );
}

template < class T, int msgType >
void PrintUserMessage( CDemoFileDump& Demo, const void *parseBuffer, int BufferSize )
{
    T msg;

    if( msg.ParseFromArray( parseBuffer, BufferSize ) )
    {
        Demo.MsgPrintf( msg, BufferSize, "%s", msg.DebugString().c_str() );
    }
}

void CDemoFileDump::DumpUserMessage( const void *parseBuffer, int BufferSize )
{
    CSVCMsg_UserMessage userMessage;

    if( userMessage.ParseFromArray( parseBuffer, BufferSize ) )
    {
        int Cmd = userMessage.msg_type();
        int SizeUM = userMessage.msg_data().size();
        const void *parseBufferUM = &userMessage.msg_data()[ 0 ];

        switch( Cmd )
        {
#define HANDLE_UserMsg( _x )            case UM_ ## _x: PrintUserMessage< CUserMsg_ ## _x, UM_ ## _x >( *this, parseBufferUM, SizeUM ); break
#define HANDLE_CS_UserMsg( _x )        case CS_UM_ ## _x: PrintUserMessage< CCSUsrMsg_ ## _x, CS_UM_ ## _x >( *this, parseBufferUM, SizeUM ); break

        default:
            printf( "WARNING. DumpUserMessage(): Unknown user message %d.\n", Cmd );
            break;

        HANDLE_CS_UserMsg( VGUIMenu );            // 1
        HANDLE_CS_UserMsg( Geiger    );                // 2
        HANDLE_CS_UserMsg( Train    );
        HANDLE_CS_UserMsg( HudText    );
        HANDLE_CS_UserMsg( SayText    );
        HANDLE_CS_UserMsg( SayText2    );
        HANDLE_CS_UserMsg( TextMsg    );
        HANDLE_CS_UserMsg( HudMsg    );
        HANDLE_CS_UserMsg( ResetHud    );
        HANDLE_CS_UserMsg( GameTitle    );
        HANDLE_CS_UserMsg( Shake    );
        HANDLE_CS_UserMsg( Fade    );
        HANDLE_CS_UserMsg( Rumble    );
        HANDLE_CS_UserMsg( CloseCaption    );
        HANDLE_CS_UserMsg( CloseCaptionDirect    );
        HANDLE_CS_UserMsg( SendAudio    );
        HANDLE_CS_UserMsg( RawAudio    );
        HANDLE_CS_UserMsg( VoiceMask    );
        HANDLE_CS_UserMsg( RequestState  );
        HANDLE_CS_UserMsg( Damage    );
        HANDLE_CS_UserMsg( RadioText    );
        HANDLE_CS_UserMsg( HintText    );
        HANDLE_CS_UserMsg( KeyHintText    );
        HANDLE_CS_UserMsg( ProcessSpottedEntityUpdate    );
        HANDLE_CS_UserMsg( ReloadEffect    );
        HANDLE_CS_UserMsg( AdjustMoney    );
        //HANDLE_CS_UserMsg( UpdateTeamMoney    );
        HANDLE_CS_UserMsg( StopSpectatorMode    );
        HANDLE_CS_UserMsg( KillCam    );
        HANDLE_CS_UserMsg( DesiredTimescale    );
        HANDLE_CS_UserMsg( CurrentTimescale    );
        HANDLE_CS_UserMsg( AchievementEvent    );
        HANDLE_CS_UserMsg( MatchEndConditions    );
        HANDLE_CS_UserMsg( DisconnectToLobby    );
        HANDLE_CS_UserMsg( PlayerStatsUpdate    );
        HANDLE_CS_UserMsg( DisplayInventory    );
        HANDLE_CS_UserMsg( WarmupHasEnded    );    
        HANDLE_CS_UserMsg( ClientInfo    );
        HANDLE_CS_UserMsg( XRankGet    );
        HANDLE_CS_UserMsg( XRankUpd    );
        HANDLE_CS_UserMsg( SetPlayerEloDisplayBracket    );
        HANDLE_CS_UserMsg( RequestEloBracketInfo    );
        HANDLE_CS_UserMsg( SetEloBracketInfo    );
        HANDLE_CS_UserMsg( CallVoteFailed    );
        HANDLE_CS_UserMsg( VoteStart    );
        HANDLE_CS_UserMsg( VotePass    );
        HANDLE_CS_UserMsg( VoteFailed    );
        HANDLE_CS_UserMsg( VoteSetup    );
        HANDLE_CS_UserMsg( ServerRankRevealAll    );
        HANDLE_CS_UserMsg( SendLastKillerDamageToClient    );
        HANDLE_CS_UserMsg( ServerRankUpdate    );
        HANDLE_CS_UserMsg( ItemPickup    );
        HANDLE_CS_UserMsg( ShowMenu    );
        HANDLE_CS_UserMsg( BarTime    );
        HANDLE_CS_UserMsg( AmmoDenied    );
        HANDLE_CS_UserMsg( MarkAchievement    );
        HANDLE_CS_UserMsg( MatchStatsUpdate    );
        HANDLE_CS_UserMsg( ItemDrop    );
#undef HANDLE_UserMsg
#undef HANDLE_CS_UserMsg
        }
    }
}

template < class T, int msgType >
void PrintNetMessage( CDemoFileDump& Demo, const void *parseBuffer, int BufferSize )
{
    T msg;

    if( msg.ParseFromArray( parseBuffer, BufferSize ) )
    {
        if( msgType == svc_GameEventList )
        {
            Demo.m_GameEventList.CopyFrom( msg );
        }

        Demo.MsgPrintf( msg, BufferSize, "%s", msg.DebugString().c_str() );
    }
}

template <>
void PrintNetMessage< CSVCMsg_UserMessage, svc_UserMessage >( CDemoFileDump& Demo, const void *parseBuffer, int BufferSize )
{
    Demo.DumpUserMessage( parseBuffer, BufferSize );
}

template <>
void PrintNetMessage< CSVCMsg_GameEvent, svc_GameEvent >( CDemoFileDump& Demo, const void *parseBuffer, int BufferSize )
{
    CSVCMsg_GameEvent msg;

    if( msg.ParseFromArray( parseBuffer, BufferSize ) )
    {
        int iDescriptor;

        for( iDescriptor = 0; iDescriptor < Demo.m_GameEventList.descriptors().size(); iDescriptor++ )
        {
            const CSVCMsg_GameEventList::descriptor_t& Descriptor = Demo.m_GameEventList.descriptors( iDescriptor );

            if( Descriptor.eventid() == msg.eventid() )
                break;
        }

        if( iDescriptor == Demo.m_GameEventList.descriptors().size() )
        {
            printf( "%s", msg.DebugString() );
        }
        else
        {
            int numKeys = msg.keys().size();
            const CSVCMsg_GameEventList::descriptor_t& Descriptor = Demo.m_GameEventList.descriptors( iDescriptor );

            printf( "%s eventid:%d %s\n", Descriptor.name().c_str(), msg.eventid(),
                msg.has_event_name() ? msg.event_name().c_str() : "" );

            for( int i = 0; i < numKeys; i++ )
            {
                const CSVCMsg_GameEventList::key_t& Key = Descriptor.keys( i );
                const CSVCMsg_GameEvent::key_t& KeyValue = msg.keys( i );

                printf(" %s: ", Key.name().c_str() );

                if( KeyValue.has_val_string() )
                    printf( "%s ", KeyValue.val_string().c_str() );
                if( KeyValue.has_val_float() )
                    printf( "%f ", KeyValue.val_float() );
                if( KeyValue.has_val_long() )
                    printf( "%d ", KeyValue.val_long() );
                if( KeyValue.has_val_short() )
                    printf( "%d ", KeyValue.val_short() );
                if( KeyValue.has_val_byte() )
                    printf( "%d ", KeyValue.val_byte() );
                if( KeyValue.has_val_bool() )
                    printf( "%d ", KeyValue.val_bool() );
                if( KeyValue.has_val_uint64() )
                    printf( "%lld ", KeyValue.val_uint64() );

                printf( "\n" );
            }
        }
    }
}

static std::string GetNetMsgName( int Cmd )
{
    if( NET_Messages_IsValid( Cmd ) )
    {
        return NET_Messages_Name( ( NET_Messages )Cmd );
    }
    else if( SVC_Messages_IsValid( Cmd ) )
    {
        return SVC_Messages_Name( ( SVC_Messages )Cmd );
    }

    assert( 0 );
    return "NETMSG_???";
}

void CDemoFileDump::DumpDemoPacket( const std::string& buf )
{
    size_t index = 0;

    while( index < buf.size() )
    {
        int Cmd = ReadVarInt32( buf, index );
        uint32 Size = ReadVarInt32( buf, index );

        if( index + Size > buf.size() )
        {
            const std::string& strName = GetNetMsgName( Cmd );

            fatal_errorf( "buf.ReadBytes() failed. Cmd:%d '%s' \n", Cmd, strName.c_str() );
        }

        switch( Cmd )
        {
#define HANDLE_NetMsg( _x )        case net_ ## _x: PrintNetMessage< CNETMsg_ ## _x, net_ ## _x >( *this, &buf[ index ], Size ); break
#define HANDLE_SvcMsg( _x )        case svc_ ## _x: PrintNetMessage< CSVCMsg_ ## _x, svc_ ## _x >( *this, &buf[ index ], Size ); break

        default:
            printf( "WARNING. DumpUserMessage(): Unknown netmessage %d.\n", Cmd );
            break;

        HANDLE_NetMsg( NOP );                // 0
        HANDLE_NetMsg( Disconnect );        // 1
        HANDLE_NetMsg( File );              // 2
        HANDLE_NetMsg( SplitScreenUser );   // 3
        HANDLE_NetMsg( Tick );              // 4
        HANDLE_NetMsg( StringCmd );         // 5
        HANDLE_NetMsg( SetConVar );         // 6
        HANDLE_NetMsg( SignonState );       // 7
        HANDLE_SvcMsg( ServerInfo );        // 8
        HANDLE_SvcMsg( SendTable );         // 9
        HANDLE_SvcMsg( ClassInfo );         // 10
        HANDLE_SvcMsg( SetPause );          // 11
        HANDLE_SvcMsg( CreateStringTable ); // 12
        HANDLE_SvcMsg( UpdateStringTable ); // 13
        HANDLE_SvcMsg( VoiceInit );         // 14
        HANDLE_SvcMsg( VoiceData );         // 15
        HANDLE_SvcMsg( Print );             // 16
        HANDLE_SvcMsg( Sounds );            // 17
        HANDLE_SvcMsg( SetView );           // 18
        HANDLE_SvcMsg( FixAngle );          // 19
        HANDLE_SvcMsg( CrosshairAngle );    // 20
        HANDLE_SvcMsg( BSPDecal );          // 21
        HANDLE_SvcMsg( SplitScreen );       // 22
        HANDLE_SvcMsg( UserMessage );       // 23
        //$ HANDLE_SvcMsg( EntityMessage ); // 24
        HANDLE_SvcMsg( GameEvent );         // 25
        HANDLE_SvcMsg( PacketEntities );    // 26
        HANDLE_SvcMsg( TempEntities );      // 27
        HANDLE_SvcMsg( Prefetch );          // 28
        HANDLE_SvcMsg( Menu );              // 29
        HANDLE_SvcMsg( GameEventList );     // 30
        HANDLE_SvcMsg( GetCvarValue );      // 31

#undef HANDLE_SvcMsg
#undef HANDLE_NetMsg
        }

        index += Size;
    }
}

static bool DumpDemoStringTable( CDemoFileDump& Demo, const CDemoStringTables& StringTables )
{
    for( int i = 0; i < StringTables.tables().size(); i++ )
    {
        const CDemoStringTables::table_t& Table = StringTables.tables( i );

        printf( "#%d %s flags:0x%x (%d Items) %d bytes\n",
            i, Table.table_name().c_str(), Table.table_flags(),
            Table.items().size() + Table.items_clientside().size(), Table.ByteSize() );

        bool bIsActiveModifiersTable = !strcmp( Table.table_name().c_str(), "ActiveModifiers" );
        bool bIsUserInfo = !strcmp( Table.table_name().c_str(), "userinfo" );

        // Only spew out the stringtables (really big) if verbose is on.
        for( int itemid = 0; itemid < Table.items().size(); itemid++ )
        {
            const CDemoStringTables::items_t& Item = Table.items( itemid );

            if( bIsActiveModifiersTable )
            {
                /*CDOTAModifierBuffTableEntry Entry;

                if( Entry.ParseFromString( Item.data() ) )
                {
                    std::string EntryStr = Entry.DebugString();
                    printf( "    #%d %s", itemid, EntryStr.c_str() );
                    continue;
                }*/
            }
            else if( bIsUserInfo && Item.data().size() == sizeof( player_info_s ) )
            {
                const player_info_s *pPlayerInfo = ( const player_info_s * )&Item.data()[ 0 ];

                printf("    xuid:%lld name:%s userID:%d guid:%s friendsID:%d friendsName:%s fakeplayer:%d ishltv:%d filesDownloaded:%d\n",
                    pPlayerInfo->xuid, pPlayerInfo->name, pPlayerInfo->userID, pPlayerInfo->guid, pPlayerInfo->friendsID,
                    pPlayerInfo->friendsName, pPlayerInfo->fakeplayer, pPlayerInfo->ishltv, pPlayerInfo->filesDownloaded );
            }

            printf( "    #%d '%s' (%d bytes)\n", itemid, Item.str().c_str(), Item.data().size() );
        }

        for( int itemid = 0; itemid < Table.items_clientside().size(); itemid++ )
        {
            const CDemoStringTables::items_t& Item = Table.items_clientside( itemid );

            printf( "    %d. '%s' (%d bytes)\n", itemid, Item.str().c_str(), Item.data().size() );
        }
    }

    return true;
}

void CDemoFileDump::PrintDemoHeader( EDemoCommands DemoCommand, int tick, int size, int uncompressed_size )
{
    const std::string& DemoCommandName = EDemoCommands_Name( DemoCommand );

    printf( "==== #%d: Tick:%d '%s' Size:%d UncompressedSize:%d ====\n",
        m_nFrameNumber, tick, DemoCommandName.c_str(), size, uncompressed_size );
}

template < class DEMCLASS >
void PrintDemoMessage( CDemoFileDump& Demo, bool bCompressed, int tick, int& size, int& uncompressed_size )
{
    DEMCLASS Msg;

    if( Demo.m_demofile.ReadMessage( &Msg, bCompressed, &size, &uncompressed_size ) )
    {
        Demo.PrintDemoHeader( Msg.GetType(), tick, size, uncompressed_size );

        Demo.MsgPrintf( Msg, size, "%s", Msg.DebugString().c_str() );
    }
}

template <>
void PrintDemoMessage<CDemoStringTables_t>( CDemoFileDump& Demo, bool bCompressed, int tick, int& size, int& uncompressed_size )
{
    CDemoStringTables_t Msg;

    if( Demo.m_demofile.ReadMessage( &Msg, bCompressed, &size, &uncompressed_size ) )
    {
        Demo.PrintDemoHeader( Msg.GetType(), tick, size, uncompressed_size );

        DumpDemoStringTable( Demo, Msg );
    }
}

void CDemoFileDump::DoDump()
{
    bool bStopReading = false;

    for( m_nFrameNumber = 0; !bStopReading; m_nFrameNumber++ )
    {
        int tick = 0;
        int size = 0;
        bool bCompressed;
        int uncompressed_size = 0;

        if( m_demofile.IsDone() )
            break;

        EDemoCommands DemoCommand = m_demofile.ReadMessageType( &tick, &bCompressed );

        switch( DemoCommand )
        {
#define HANDLE_DemoMsg( _x )    case DEM_ ## _x: PrintDemoMessage< CDemo ## _x ## _t >( *this, bCompressed, tick, size, uncompressed_size ); break

        HANDLE_DemoMsg( SyncTick );
        HANDLE_DemoMsg( ConsoleCmd );
        HANDLE_DemoMsg( UserCmd );
        HANDLE_DemoMsg( DataTables );
        HANDLE_DemoMsg( Stop );
        HANDLE_DemoMsg( CustomData );
        HANDLE_DemoMsg( StringTables );

#undef HANDLE_DemoMsg

        case DEM_Packet:
        case DEM_SignonPacket:
            {
                CDemoPacket_t Packet;

                if( m_demofile.ReadMessage( &Packet, bCompressed, &size, &uncompressed_size ) )
                {
                    PrintDemoHeader( DemoCommand, tick, size, uncompressed_size );

                    DumpDemoPacket( Packet.data() );
                }
            }
            break;

        default:
        case DEM_Error:
            bStopReading = true;
            fatal_errorf( "Shouldn't ever get this demo command?!? %d\n", DemoCommand );
            break;
        }

        if( _kbhit() )
        {
            int ch = toupper( _getch() );

            if( ch == 'Q' )
                break;
        }
    }
}