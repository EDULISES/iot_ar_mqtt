//-----------------------------------------------------------------------------
//  File:       exitApp.cs
//  
//	Author:		Edward U. Benitez Rendon
//  
//  Date:		11.07.2017
//  
//	Desc:		The file contains functions to exit from application.
//-----------------------------------------------------------------------------
using UnityEngine.UI;
using UnityEngine;
// MQTT libraries
using uPLibrary.Networking.M2Mqtt;
using System;
using System.Text;
using uPLibrary.Networking.M2Mqtt.Messages;
using Vuforia;

public class exitApp : MonoBehaviour {
    private MqttClient4Unity client;
    // Data to connect with the mqtt broker
    public string brokerHostname = "255.255.255.255";
    public int brokerPort = 1883;
    public string usrNm = null;
    public string psw = null;
    public string topic = "zone/sensor";

    // Use this for initialization
    void Start()
    {
        // Connect to the MQTT broker
        if (brokerHostname != null && usrNm != null && psw != null)
        {
            Connect();
        }
    }

    // Exit from application
    public void exitAppOnClick()
    {
        client.Publish(topic, System.Text.Encoding.ASCII.GetBytes("0"));
        Application.Quit();
    }

    /// <summary>
    /// Conect mqtt client to the broker
    /// </summary>
    public void Connect()
    {
        client = new MqttClient4Unity(brokerHostname, brokerPort, false, null);
        string clientId = Guid.NewGuid().ToString();
        client.Connect(clientId, usrNm, psw);
    }

    /// <summary>
    /// Publish payload to the broker
    /// </summary>
    /// <param name="_topic">The topic to send</param>
    /// <param name="msg">Payload</param>
    public void Publish(string _topic, string msg)
    {
        client.Publish(_topic, Encoding.UTF8.GetBytes(msg), MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE, false);
    }
}
