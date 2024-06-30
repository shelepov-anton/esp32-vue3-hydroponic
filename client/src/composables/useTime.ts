import { computed, onUnmounted, ref } from 'vue'


const dateOptions = {
  month: 'long',
  day: 'numeric',
  timeZone: 'UTC'
};

const timeOptions = {
  hour: 'numeric',
  minute: 'numeric',
};

export default () => {
  const date = ref<Date>(new Date())

  const updateDate = () => {
    date.value = new Date()
  }

  const timer = setInterval(() => {
    updateDate()
  }, 1000)

  onUnmounted(() => {
    clearInterval(timer)
  })

  const formattedTime = computed<{date: string, time: string}>(() => ({
    date: date.value.toLocaleString("ru", dateOptions as any),
    time: date.value.toLocaleTimeString("ru", timeOptions as any)
  }))

  return {
    formattedTime,
  }
}