import { defineStore } from 'pinia'
import { ref } from 'vue'

export const useConfigurationStore = defineStore('configuration', () =>{
  const daysCount = ref<number>(0)

  // Notification
  const isNotificationHidden = ref<boolean>(true)
  const notificationMessages = ref<string[]>([])

  const hideNotification = (seconds: number = 15) => {
    isNotificationHidden.value = true
    setTimeout(() => {
      isNotificationHidden.value = false
    }, seconds * 1000)
  }

  const showNotification = (message: string) => {
    console.log(message)
    const isNotificationAlreadyExist = notificationMessages.value.includes(message)
    if (isNotificationHidden.value && !isNotificationAlreadyExist) {
      isNotificationHidden.value = false
    }
    if (!isNotificationAlreadyExist) {
      notificationMessages.value.push(message)
    }
  }

  return {
    daysCount,
    notificationMessages,
    isNotificationHidden,
    hideNotification,
    showNotification
  }
})